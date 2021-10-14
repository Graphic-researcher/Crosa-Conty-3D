#pragma once

/// You can change these key code map based on your platform
/// Or using: #ifdef CC3D_PLATFORM_WINDOWS	... #endif
/// Our key code map is the same as glfw map,so we don't need to write a 
/// convert function to translate our key code to glfw key code map
/// However,if you work in another platform using different key code map
/// Then you should have a convert function to glfw key code map
/// For example :
/// glfwGetKey(window,CC3DKeyCodeToGLFWKeyCode(keycode));

#ifdef CC3D_PLATFORM_WINDOWS
// From glfw3.h
	#define CC3D_KEY_SPACE              32
	#define CC3D_KEY_APOSTROPHE         39  /* ' */
	#define CC3D_KEY_COMMA              44  /* , */
	#define CC3D_KEY_MINUS              45  /* - */
	#define CC3D_KEY_PERIOD             46  /* . */
	#define CC3D_KEY_SLASH              47  /* / */
	#define CC3D_KEY_0                  48
	#define CC3D_KEY_1                  49
	#define CC3D_KEY_2                  50
	#define CC3D_KEY_3                  51
	#define CC3D_KEY_4                  52
	#define CC3D_KEY_5                  53
	#define CC3D_KEY_6                  54
	#define CC3D_KEY_7                  55
	#define CC3D_KEY_8                  56
	#define CC3D_KEY_9                  57
	#define CC3D_KEY_SEMICOLON          59  /* ; */
	#define CC3D_KEY_EQUAL              61  /* = */
	#define CC3D_KEY_A                  65
	#define CC3D_KEY_B                  66
	#define CC3D_KEY_C                  67
	#define CC3D_KEY_D                  68
	#define CC3D_KEY_E                  69
	#define CC3D_KEY_F                  70
	#define CC3D_KEY_G                  71
	#define CC3D_KEY_H                  72
	#define CC3D_KEY_I                  73
	#define CC3D_KEY_J                  74
	#define CC3D_KEY_K                  75
	#define CC3D_KEY_L                  76
	#define CC3D_KEY_M                  77
	#define CC3D_KEY_N                  78
	#define CC3D_KEY_O                  79
	#define CC3D_KEY_P                  80
	#define CC3D_KEY_Q                  81
	#define CC3D_KEY_R                  82
	#define CC3D_KEY_S                  83
	#define CC3D_KEY_T                  84
	#define CC3D_KEY_U                  85
	#define CC3D_KEY_V                  86
	#define CC3D_KEY_W                  87
	#define CC3D_KEY_X                  88
	#define CC3D_KEY_Y                  89
	#define CC3D_KEY_Z                  90
	#define CC3D_KEY_LEFT_BRACKET       91  /* [ */
	#define CC3D_KEY_BACKSLASH          92  /* \ */
	#define CC3D_KEY_RIGHT_BRACKET      93  /* ] */
	#define CC3D_KEY_GRAVE_ACCENT       96  /* ` */
	#define CC3D_KEY_WORLD_1            161 /* non-US #1 */
	#define CC3D_KEY_WORLD_2            162 /* non-US #2 */

	/* Function keys */
	#define CC3D_KEY_ESCAPE             256
	#define CC3D_KEY_ENTER              257
	#define CC3D_KEY_TAB                258
	#define CC3D_KEY_BACKSPACE          259
	#define CC3D_KEY_INSERT             260
	#define CC3D_KEY_DELETE             261
	#define CC3D_KEY_RIGHT              262
	#define CC3D_KEY_LEFT               263
	#define CC3D_KEY_DOWN               264
	#define CC3D_KEY_UP                 265
	#define CC3D_KEY_PAGE_UP            266
	#define CC3D_KEY_PAGE_DOWN          267
	#define CC3D_KEY_HOME               268
	#define CC3D_KEY_END                269
	#define CC3D_KEY_CAPS_LOCK          280
	#define CC3D_KEY_SCROLL_LOCK        281
	#define CC3D_KEY_NUM_LOCK           282
	#define CC3D_KEY_PRINT_SCREEN       283
	#define CC3D_KEY_PAUSE              284
	#define CC3D_KEY_F1                 290
	#define CC3D_KEY_F2                 291
	#define CC3D_KEY_F3                 292
	#define CC3D_KEY_F4                 293
	#define CC3D_KEY_F5                 294
	#define CC3D_KEY_F6                 295
	#define CC3D_KEY_F7                 296
	#define CC3D_KEY_F8                 297
	#define CC3D_KEY_F9                 298
	#define CC3D_KEY_F10                299
	#define CC3D_KEY_F11                300
	#define CC3D_KEY_F12                301
	#define CC3D_KEY_F13                302
	#define CC3D_KEY_F14                303
	#define CC3D_KEY_F15                304
	#define CC3D_KEY_F16                305
	#define CC3D_KEY_F17                306
	#define CC3D_KEY_F18                307
	#define CC3D_KEY_F19                308
	#define CC3D_KEY_F20                309
	#define CC3D_KEY_F21                310
	#define CC3D_KEY_F22                311
	#define CC3D_KEY_F23                312
	#define CC3D_KEY_F24                313
	#define CC3D_KEY_F25                314
	#define CC3D_KEY_KP_0               320
	#define CC3D_KEY_KP_1               321
	#define CC3D_KEY_KP_2               322
	#define CC3D_KEY_KP_3               323
	#define CC3D_KEY_KP_4               324
	#define CC3D_KEY_KP_5               325
	#define CC3D_KEY_KP_6               326
	#define CC3D_KEY_KP_7               327
	#define CC3D_KEY_KP_8               328
	#define CC3D_KEY_KP_9               329
	#define CC3D_KEY_KP_DECIMAL         330
	#define CC3D_KEY_KP_DIVIDE          331
	#define CC3D_KEY_KP_MULTIPLY        332
	#define CC3D_KEY_KP_SUBTRACT        333
	#define CC3D_KEY_KP_ADD             334
	#define CC3D_KEY_KP_ENTER           335
	#define CC3D_KEY_KP_EQUAL           336
	#define CC3D_KEY_LEFT_SHIFT         340
	#define CC3D_KEY_LEFT_CONTROL       341
	#define CC3D_KEY_LEFT_ALT           342
	#define CC3D_KEY_LEFT_SUPER         343
	#define CC3D_KEY_RIGHT_SHIFT        344
	#define CC3D_KEY_RIGHT_CONTROL      345
	#define CC3D_KEY_RIGHT_ALT          346
	#define CC3D_KEY_RIGHT_SUPER        347
	#define CC3D_KEY_MENU               348

#endif