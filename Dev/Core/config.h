#ifndef ENN_CONFIG_H
#define ENN_CONFIG_H

#ifdef __cplusplus
        extern "C" {
#endif

 /* ----- Base Configs -----*/
 
  #define ENN_DEBUG_MODE

  #define ENN_APP_DIRECTORY "Game"
//   #define ENN_DATA_PATH "/Data/"
//   #define ENN_LOGS_PATH "/Data/Logs/"

//   #define ENN_GENERAL_LOG_FILENAME_FORMAT "%Y-%m-%d-%H-%M-%S.log"
//   #define ENN_MEMORY_LOG_FILENAME_FORMAT "%Y-%m-%d-%H-%M-%S.memlog"
//   #define ENN_LOG_TIME_FORMAT "[%H:%M:%S]: "

//   #define ENN_MAX_LOG_BUFF_SIZE 2048
  #define ENN_MAX_CALLSTACK_SIZE 64

//   #define ENN_VECTOR_IMPL
//   #define ENN_DEQUE_IMPL

//   #define ENN_VECTOR_INIT_CAPACITY 8
//   #define ENN_DEQUE_INIT_CAPACITY 16
 
//   #define ENN_DATAFILE_KEYPATH_SEPARATOR           '|'
//   #define ENN_DATAFILE_OPERATOR_ASSIGN             '='
//   #define ENN_DATAFILE_STRING_IDENTIFIER           '"'
//   #define ENN_DATAFILE_OPERATOR_LIST_BEGIN         '['
//   #define ENN_DATAFILE_OPERATOR_LIST_END           ']'
//   #define ENN_DATAFILE_OPERATOR_LIST_CONTINUE      ','


//   #define ENN_DATAFILE_LIST_ITEM_PADDING           4
//   #define ENN_DATAFILE_FILE_EXTENSION              ".enn"
//   #define ENN_DATAFILE_WHITESPACE                  " \n\t"
//   #define ENN_DATAFILE_NUMBER_SYMBOLS              "0123456789.-"
//   #define ENN_DATAFILE_MAX_STRING_SIZE 64

 
/* ----- Optional Configs ----- */
  #define ENN_INCLUDE_OPTIONAL
//   #define ENN_MULTI_WINDOW
//   #define ENN_FOCUS_WINDOW_OPTIMIZATION

//   #define ENN_WINDOW_MIN_WIDTH 320
//   #define ENN_WINDOW_MIN_HEIGHT 180

  #define ENN_USE_CUSTOM_RENDERING
//   #define ENN_DEFAULT_RENDERER_VERTEX_BUFF_SIZE 2048


//   #define ENN_USER_DEFINED_EVENTS               
//   #define ENN_ASSETS_PATH "/Assets/"
//   #define ENN_SHADERS_PATH "/Assets/Shaders/"
//   #define ENN_TEXTURES_PATH "/Assets/Textures/"
//   #define ENN_FONTS_PATH "/Assets/Fonts/"

#ifdef __cplusplus
        }
#endif

#endif
