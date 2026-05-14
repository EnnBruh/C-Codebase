#include "Base/base_internal.h"

ENNDEF_PUBLIC bool is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

ENNDEF_PUBLIC bool is_number_symbol(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == '-';
}

ENNDEF_PUBLIC void log_serialization_error(const char* msg, const char* filepath, const char* buffer_start, const char* cursor) {
    i32 line = 1;
    for (const char* c = buffer_start; c < cursor; ++c) {
        if (*c == '\n') line++;
    }

    char context[24] = {0};
    for (i32 i = 0; i < 20 && cursor[i] != '\0'; ++i) {
        context[i] = (cursor[i] == '\n' || cursor[i] == '\r') ? ' ' : cursor[i];
    }

    LOG_WARN("[Serialization] %s at line %d in %s. Context: '%s'", 
                   msg, (int)line, filepath ? filepath : "abstract_buffer", context);
}

void datafile_create(DataFile* df) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    memset(df, 0, (sizeof (DataFile)));
    DEBUG_UNTRACE();
}

static void datafile_destroy_node(DataFileNode* node) {
    DEBUG_TRACE();
    if (!node) {
        DEBUG_UNTRACE();
        return;
    }
    if (node -> type == ENN_LIST) {
        for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
            datafile_destroy_node(node -> data.children.data[i]);
        }
        vector_destroy(node -> data.children);
    }
    free(node);
    DEBUG_UNTRACE();
}

void datafile_destroy(DataFile* df) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    datafile_destroy_node(df -> root);
    df -> root = NULL;
    free(df -> filepath);
    DEBUG_UNTRACE();
}

static DataFileNode* datafile_parse_node(DataFileNode* parent, char** cursor, const char* filepath, const char* buffer_start) {
    DEBUG_TRACE();
    DEBUG_ASSERT(cursor != NULL);

    DataFileNode* node = calloc(1, (sizeof (DataFileNode)));
    if (!node) {
        log_serialization_error("Memory allocation failed for DataFileNode", filepath, buffer_start, *cursor);
        DEBUG_UNTRACE();
        return NULL;
    }
    node -> parent = parent;

    while (**cursor && is_whitespace(**cursor)) ++(*cursor);
    if (!**cursor) {
        datafile_destroy_node(node);
        DEBUG_UNTRACE();
        return NULL;
    }

    if (**cursor != ENN_DATAFILE_STRING_IDENTIFIER) {
        log_serialization_error("Expected string identifier '\"'", filepath, buffer_start, *cursor);
        datafile_destroy_node(node);
        DEBUG_UNTRACE();
        return NULL;
    }

    char* start = ++(*cursor);
    while (**cursor && **cursor != ENN_DATAFILE_STRING_IDENTIFIER) ++(*cursor);
    if (!**cursor) {
        log_serialization_error("Unterminated string identifier for key", filepath, buffer_start, *cursor);
        datafile_destroy_node(node);
        DEBUG_UNTRACE();
        return NULL;
    }

    i32 len = (*cursor) - start;
    if (len >= ENN_DATAFILE_MAX_STRING_SIZE) len = ENN_DATAFILE_MAX_STRING_SIZE - 1;
    memcpy(node -> key, start, len);
    ++(*cursor);

    while (**cursor && is_whitespace(**cursor)) ++(*cursor);

    if (**cursor != ENN_DATAFILE_OPERATOR_ASSIGN) {
        log_serialization_error("Expected assign operator '='", filepath, buffer_start, *cursor);
        datafile_destroy_node(node);
        DEBUG_UNTRACE();
        return NULL;
    }
    ++(*cursor);

    while (**cursor && is_whitespace(**cursor)) ++(*cursor);

    if (**cursor == ENN_DATAFILE_STRING_IDENTIFIER) {
        start = ++(*cursor);
        while (**cursor && **cursor != ENN_DATAFILE_STRING_IDENTIFIER) ++(*cursor);
        if (!**cursor) {
            char err_buf[128];
            snprintf(err_buf, (sizeof err_buf), "Unterminated string value for key '%s'", node -> key);
            log_serialization_error(err_buf, filepath, buffer_start, *cursor);
            datafile_destroy_node(node);
            DEBUG_UNTRACE();
            return NULL;
        }

        len = (*cursor) - start;
        if (len >= ENN_DATAFILE_MAX_STRING_SIZE) len = ENN_DATAFILE_MAX_STRING_SIZE - 1;

        node -> type = ENN_STRING;
        memcpy(node -> data.string_val, start, len);
        ++(*cursor);

    } else if (**cursor == ENN_DATAFILE_OPERATOR_LIST_BEGIN) {
        ++(*cursor);
        node -> type = ENN_LIST;
        
        while (**cursor) {
            while (**cursor && is_whitespace(**cursor)) ++(*cursor);
            if (**cursor == ENN_DATAFILE_OPERATOR_LIST_END) {
                ++(*cursor);
                break;
            }

            DataFileNode* child = datafile_parse_node(node, cursor, filepath, buffer_start);
            if (!child) {
                char err_buf[128];
                snprintf(err_buf, (sizeof err_buf), "Failed to parse child node in list for key '%s'", node -> key);
                log_serialization_error(err_buf, filepath, buffer_start, *cursor);
                datafile_destroy_node(node);
                DEBUG_UNTRACE();
                return NULL;
            }
            vector_push_back(node -> data.children, child);

            while (**cursor && is_whitespace(**cursor)) ++(*cursor);
            if (**cursor == ENN_DATAFILE_OPERATOR_LIST_CONTINUE) {
                ++(*cursor);
            } else if (**cursor != ENN_DATAFILE_OPERATOR_LIST_END) {
                log_serialization_error("Expected list continuation ',' or end ']'", filepath, buffer_start, *cursor);
                datafile_destroy_node(node);
                DEBUG_UNTRACE();
                return NULL;
            }
        }
    } else if (is_number_symbol(**cursor)) {
        char* scan = *cursor;
        bool is_real = false;
        while (*scan && is_number_symbol(*scan)) {
            if (*scan == '.') is_real = true;
            ++scan;
        }

        if (is_real) {
            node -> type = ENN_REAL;
            node -> data.real_val = strtof(*cursor, cursor);
        } else {
            node -> type = ENN_INT;
            node -> data.int_val = strtol(*cursor, cursor, 10);
        }
    } else {
        char err_buf[128];
        snprintf(err_buf, (sizeof err_buf), "Unknown data type for key '%s'", node -> key);
        log_serialization_error(err_buf, filepath, buffer_start, *cursor);
        datafile_destroy_node(node);
        DEBUG_UNTRACE();
        return NULL;
    }

    DEBUG_UNTRACE();
    return node;
}

static i32 datafile_write_node_precalc(DataFileNode* node, i32 depth) {
    DEBUG_TRACE();
    i32 to_be_written = 0;
    switch (node -> type) {
        case ENN_INT:
            to_be_written += snprintf(NULL, 0, "%*c%s%c %c %" PRIi32, ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, node -> data.int_val);
            break;
        case ENN_REAL:
            to_be_written += snprintf(NULL, 0, "%*c%s%c %c %f", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, node -> data.real_val);
            break;
        case ENN_STRING:
            to_be_written += snprintf(NULL, 0, "%*c%s%c %c %c%s%c", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, ENN_DATAFILE_STRING_IDENTIFIER, node -> data.string_val, ENN_DATAFILE_STRING_IDENTIFIER);
            break;
        case ENN_LIST: {
            to_be_written += snprintf(NULL, 0, "%*c%s%c %c %c\n", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, ENN_DATAFILE_OPERATOR_LIST_BEGIN);
            for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
                to_be_written += datafile_write_node_precalc(node -> data.children.data[i], depth + 1);
                if (i + 1 < node -> data.children.end) to_be_written += snprintf(NULL, 0, "%c\n", ENN_DATAFILE_OPERATOR_LIST_CONTINUE);
                else to_be_written += snprintf(NULL, 0, "\n");
            }
            to_be_written += snprintf(NULL, 0, "%*c", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_OPERATOR_LIST_END);
            break;
        }
    }
    DEBUG_UNTRACE();
    return to_be_written;
}

static void datafile_write_node(DataFileNode* node, i32 depth, char** cursor) {
    DEBUG_TRACE();
    switch (node -> type) {
        case ENN_INT:
            *cursor += sprintf(*cursor, "%*c%s%c %c %" PRIi32, ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, node -> data.int_val);
            break;
        case ENN_REAL:
            *cursor += sprintf(*cursor, "%*c%s%c %c %f", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, node -> data.real_val);
            break;
        case ENN_STRING:
            *cursor += sprintf(*cursor, "%*c%s%c %c %c%s%c", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, ENN_DATAFILE_STRING_IDENTIFIER, node -> data.string_val, ENN_DATAFILE_STRING_IDENTIFIER);
            break;
        case ENN_LIST: {
            *cursor += sprintf(*cursor, "%*c%s%c %c %c\n", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_STRING_IDENTIFIER, node -> key, ENN_DATAFILE_STRING_IDENTIFIER, ENN_DATAFILE_OPERATOR_ASSIGN, ENN_DATAFILE_OPERATOR_LIST_BEGIN);
            for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
                datafile_write_node(node -> data.children.data[i], depth + 1, cursor);
                if (i + 1 < node -> data.children.end) *cursor += sprintf(*cursor, "%c\n", ENN_DATAFILE_OPERATOR_LIST_CONTINUE);
                else *cursor += sprintf(*cursor, "\n");
            }
            *cursor += sprintf(*cursor, "%*c", ENN_DATAFILE_LIST_ITEM_PADDING * depth + 1, ENN_DATAFILE_OPERATOR_LIST_END);
            break;
        }
    }
    DEBUG_UNTRACE();
}

DataFileNode* datafile_parse_keypath_find_or_create_node(DataFile* df, char* keypath) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(keypath != NULL);

    DataFileNode* root = df -> root;
    char* cursor = keypath;
    char* token = cursor;
    
    while (*cursor && *cursor != ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
    i32 len = cursor - token;
    if (len >= ENN_DATAFILE_MAX_STRING_SIZE) len = ENN_DATAFILE_MAX_STRING_SIZE - 1;

    if (!root) {
        root = calloc(1, (sizeof (DataFileNode)));
        memcpy(root -> key, token, len);
        df -> root = root;
    } else if (strncmp(token, root -> key, len) != 0 || root -> key[len] != '\0') {
        DataFileNode* new_node = calloc(1, (sizeof (DataFileNode)));
        memcpy(new_node -> key, token, len);
        new_node -> parent = root;
        
        if (root -> type != ENN_LIST) root -> type = ENN_LIST;
        vector_push_back(root -> data.children, new_node);
        root = new_node;
    }

    if (*cursor == ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;

    while (*cursor) {
        token = cursor;
        while (*cursor && *cursor != ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
        len = cursor - token;
        if (len >= ENN_DATAFILE_MAX_STRING_SIZE) len = ENN_DATAFILE_MAX_STRING_SIZE - 1;

        bool found = false;
        if (root -> type != ENN_LIST) root -> type = ENN_LIST;

        for (i32 i = root -> data.children.start; i < root -> data.children.end; ++i) {
            if (strncmp(token, root -> data.children.data[i] -> key, len) == 0 && root -> data.children.data[i] -> key[len] == '\0') {
                found = true;
                root = root -> data.children.data[i];
                break;
            }
        }

        if (!found) {
            DataFileNode* new_node = calloc(1, (sizeof (DataFileNode)));
            memcpy(new_node -> key, token, len);
            new_node -> parent = root;
            vector_push_back(root -> data.children, new_node);
            root = new_node;
        }

        if (*cursor == ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
    }

    DEBUG_UNTRACE();
    return root;
}

void datafile_put_i32(DataFile* df, char* keypath, i32 val) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_or_create_node(df, keypath);

    if (node -> type == ENN_LIST) {
        for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
            datafile_destroy_node(node -> data.children.data[i]);
        }
        vector_destroy(node -> data.children);
    }

    node -> type = ENN_INT;
    node -> data.int_val = val;
    DEBUG_UNTRACE();
}

void datafile_put_f32(DataFile* df, char* keypath, f32 val) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_or_create_node(df, keypath);

    if (node -> type == ENN_LIST) {
        for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
            datafile_destroy_node(node -> data.children.data[i]);
        }
        vector_destroy(node -> data.children);
    }

    node -> type = ENN_REAL;
    node -> data.real_val = val;
    DEBUG_UNTRACE();
}

void datafile_put_cstring(DataFile* df, char* keypath, const char* val) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_or_create_node(df, keypath);

    if (node -> type == ENN_LIST) {
        for (i32 i = node -> data.children.start; i < node -> data.children.end; ++i) {
            datafile_destroy_node(node -> data.children.data[i]);
        }
        vector_destroy(node -> data.children);
    }

    node -> type = ENN_STRING;
    i32 size = strlen(val);
    if (size >= ENN_DATAFILE_MAX_STRING_SIZE) size = ENN_DATAFILE_MAX_STRING_SIZE - 1;
    memset(node -> data.string_val, 0, ENN_DATAFILE_MAX_STRING_SIZE);
    memcpy(node -> data.string_val, val, size);
    DEBUG_UNTRACE();
}

DataFileNode* datafile_parse_keypath_find_node(DataFile* df, char* keypath) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(keypath != NULL);

    DataFileNode* root = df -> root;
    char* cursor = keypath;
    char* token = cursor;
    
    while (*cursor && *cursor != ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
    i32 len = cursor - token;

    if (!root || strncmp(token, root -> key, len) != 0 || root -> key[len] != '\0') {
        DEBUG_UNTRACE();
        return NULL;
    }

    if (*cursor == ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;

    while (*cursor) {
        token = cursor;
        while (*cursor && *cursor != ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
        len = cursor - token;

        bool found = false;
        if (root -> type != ENN_LIST) {
            DEBUG_UNTRACE();
            return NULL;
        }

        for (i32 i = root -> data.children.start; i < root -> data.children.end; ++i) {
            if (strncmp(token, root -> data.children.data[i] -> key, len) == 0 && root -> data.children.data[i] -> key[len] == '\0') {
                found = true;
                root = root -> data.children.data[i];
                break;
            }
        }

        if (!found) {
            DEBUG_UNTRACE();
            return NULL;
        }

        if (*cursor == ENN_DATAFILE_KEYPATH_SEPARATOR) ++cursor;
    }

    DEBUG_UNTRACE();
    return root;
}

i32 datafile_get_i32(DataFile* df, char* keypath) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_node(df, keypath);
    DEBUG_UNTRACE();
    if (node && node -> type == ENN_INT) return node -> data.int_val;
    LOG_WARN("[Serialization] No i32 value was found for key '%s' in '%s'", keypath, df -> filepath);
    return 0;
}

f32 datafile_get_f32(DataFile* df, char* keypath) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_node(df, keypath);
    DEBUG_UNTRACE();
    if (node && node -> type == ENN_REAL) return node -> data.real_val;
    LOG_WARN("[Serialization] No f32 value was found for key '%s' in '%s'", keypath, df -> filepath);
    return 0.0f;
}

char* datafile_get_cstring(DataFile* df, char* keypath) {
    DEBUG_TRACE();
    DataFileNode* node = datafile_parse_keypath_find_node(df, keypath);
    DEBUG_UNTRACE();
    if (node && node -> type == ENN_STRING) return node -> data.string_val;
    LOG_WARN("[Serialization] No cstring value was found for key '%s' in '%s'", keypath, df -> filepath);
    return NULL;
}

void datafile_read(DataFile* df, const char* filepath) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(filepath != NULL);

    i32 path_len = strlen(filepath);
    ASSERT(path_len >= (sizeof ENN_DATAFILE_FILE_EXTENSION) && 
           strcmp(filepath + path_len - (sizeof ENN_DATAFILE_FILE_EXTENSION) + 1, ENN_DATAFILE_FILE_EXTENSION) == 0,
           "[Serialization] File %s is unsupported. Only supported file type is %s", filepath, ENN_DATAFILE_FILE_EXTENSION);

    df -> filepath = calloc(path_len, (sizeof (char)));
    memcpy(df -> filepath, filepath, path_len * (sizeof (char)));

    char* file_data;
    file_read_cstring(filepath, &file_data);

    char* copy = file_data;
    df -> root = datafile_parse_node(NULL, &copy, filepath, file_data);
    free(file_data);
    DEBUG_UNTRACE();
}

void datafile_read_abstract(DataFile* df, const char* buff) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(buff != NULL);

    char* copy = (char*)buff;
    df -> root = datafile_parse_node(NULL, &copy, "abstract_buffer", buff);
    DEBUG_UNTRACE();
}

i32 datafile_write_precalc(DataFile* df) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    i32 calc = df -> root ? datafile_write_node_precalc(df -> root, 0) : 0;
    DEBUG_UNTRACE();
    return calc;
}

void datafile_write_abstract(DataFile* df, const char* buff) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(buff != NULL);

    if (df -> root) {
        char* copy = (char*)buff;
        datafile_write_node(df -> root, 0, &copy);
    }
    DEBUG_UNTRACE();
}

void datafile_write(DataFile* df, const char* filepath) {
    DEBUG_TRACE();
    DEBUG_ASSERT(df != NULL);
    DEBUG_ASSERT(filepath != NULL);

    if (!df -> root) {
        DEBUG_UNTRACE();
        return;
    }

    i32 reserve_size = datafile_write_node_precalc(df -> root, 0);
    DEBUG_ASSERT(reserve_size > 0);

    char* file_data = calloc(reserve_size + 1, (sizeof (char)));
    char* copy = file_data;
    datafile_write_node(df -> root, 0, &copy);

    file_write_cstring(filepath, file_data, reserve_size);
    free(file_data);
    DEBUG_UNTRACE();
}