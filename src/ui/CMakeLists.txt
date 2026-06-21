SET(SOURCES screens/ui_scrMain.c
    screens/ui_scrSettings.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_bkgrnd5_png.c
    images/ui_img_battt_png.c
    images/ui_img_963419959.c
    images/ui_img_cruise_png.c
    images/ui_img_258024724.c
    images/ui_img_1559759506.c
    images/ui_img_speedometer_png.c
    images/ui_img_1698700558.c
    images/ui_img_1930944701.c
    fonts/ui_font_Moscow14.c
    fonts/ui_font_Moscow16.c
    fonts/ui_font_Moscow24.c
    fonts/ui_font_Moscow28.c)

add_library(ui ${SOURCES})
