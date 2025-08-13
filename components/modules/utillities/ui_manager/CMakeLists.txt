# UI Manager Module CMake Configuration
# Component for Manages the user interface, including menus and status screens.
# Author: Giorgi Magradze
# Version: 1.0.0

# SMART CONDITIONAL COMPILATION SYSTEM:
# 1. თუ CONFIG variable ჯერ არ არსებობს (configure ეტაპი), რეგისტრირდება სრული ფუნქციონალით.
# 2. თუ CONFIG variable არსებობს და ჩართულია, რეგისტრირდება სრული ფუნქციონალით.
# 3. თუ CONFIG variable არსებობს და გამორთულია, რეგისტრირდება ცარიელი placeholder-ით.

if(NOT DEFINED CONFIG_MODULE_UI_MANAGER_ENABLED OR CONFIG_MODULE_UI_MANAGER_ENABLED)
    # მოდული ჩართულია ან ჯერ არ არის კონფიგურირებული
    if(NOT DEFINED CONFIG_MODULE_UI_MANAGER_ENABLED)
        message(STATUS "UI Manager Module: CONFIGURE STAGE - დროებითი რეგისტრაცია")
    else()
        message(STATUS "UI Manager Module: ENABLED - კომპილირდება სრული ფუნქციონალით")
    endif()
    
    idf_component_register(
        SRCS "src/ui_manager.c"
            "src/ui_manager_internal.h"
            "src/ui_assets.c"
            "src/ui_menu.c"
            "src/ui_events.c"
            "src/ui_screens.c"
            # "src/ui_status_screen.c"
            # "src/ui_notification.c"
            # "src/ui_input.c"
            # "src/ui_theme.c"
            # "src/ui_event_handler.c"
            # "src/ui_animation.c"
            # "src/ui_layout.c"
            # "src/ui_widget.c"
            # "src/ui_screen_manager.c"
            # "src/ui_touch_input.c"
            # "src/ui_keyboard.c"
            # "src/ui_font.c"
            # "src/ui_icon.c"
            # "src/ui_color_palette.c"
            # "src/ui_progress_bar.c"
            # "src/ui_button.c"
            # "src/ui_slider.c"
            # "src/ui_checkbox.c"
            # "src/ui_text_input.c"
            # "src/ui_list_view.c"
            # "src/ui_dialog.c"
            # "src/ui_tooltip.c"
            # "src/ui_scroll_view.c"
            # "src/ui_tab_bar.c"
            # "src/ui_switch.c"
            # "src/ui_image_view.c"
            # "src/ui_loading_spinner.c"
            # "src/ui_status_bar.c"
            # "src/ui_battery_indicator.c"
            # "src/ui_network_indicator.c"
            # "src/ui_notification_center.c"
            # "src/ui_progress_indicator.c"
            # "src/ui_search_bar.c"
            # "src/ui_date_time_picker.c"
            # "src/ui_color_picker.c"
            # "src/ui_menu_item.c"
            # "src/ui_menu_bar.c"
            # "src/ui_menu_popup.c"
            # "src/ui_menu_context.c"
            # "src/ui_menu_shortcut.c"
            # "src/ui_menu_navigation.c"
            # "src/ui_menu_action.c"
            # "src/ui_menu_selection.c"
            # "src/ui_menu_search.c"
            # "src/ui_menu_filter.c"
            # "src/ui_menu_sort.c"
            # "src/ui_menu_settings.c"
            # "src/ui_menu_help.c"
            # "src/ui_menu_about.c"
            # "src/ui_menu_exit.c"
            # "src/ui_menu_confirm.c"
            # "src/ui_menu_cancel.c"
            # "src/ui_menu_save.c"
            # "src/ui_menu_load.c"
            # "src/ui_menu_reset.c"
            # "src/ui_menu_apply.c"
            # "src/ui_menu_close.c"
        INCLUDE_DIRS "include"
            REQUIRES 
                core 
                interfaces
        PRIV_REQUIRES
            json
    )
else()
    # მოდული გამორთულია
    message(STATUS "UI Manager Module: DISABLED - კომპილირდება ცარიელი placeholder-ით")
    
    set(EMPTY_SOURCE_CONTENT "// ui_manager module disabled by Kconfig\n// This is an empty placeholder to prevent CMake errors.\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/empty_ui_manager.c" "${EMPTY_SOURCE_CONTENT}")
    
    idf_component_register(
        SRCS "${CMAKE_CURRENT_BINARY_DIR}/empty_ui_manager.c"
        INCLUDE_DIRS "include"
        REQUIRES core interfaces
    )
endif()
