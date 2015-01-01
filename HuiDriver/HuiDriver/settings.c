#include "settings.h"

void loadSettings() {
    config_t cfg;
    config_setting_t *setting;
    const char *str;

    config_init(&cfg);

    unsigned int count;
    unsigned int i;

    /* Read the file. If there is an error, report it and exit. */
    if(! config_read_file(&cfg, "/Users/adonis/Documents/Projekte/C/Hui/HuiDriver/HuiDriver/currentTablet.cfg"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
    }

    config_lookup_int(&cfg, "leftMode", &leftMode);


    setting = config_lookup(&cfg, "penButtons");
    if(setting != NULL)
    {

        count = config_setting_length(setting);
        for(i = 0; i < count; ++i)
        {
            config_setting_t *penButton = config_setting_get_elem(setting, i);
//
//            /* Only output the record if all of the expected fields are present. */
//            const char *title, *media;
//            double price;
            int input, mode, cmd, ctrl, alt, shift, key;
//
            if(!(config_setting_lookup_int(penButton, "cmd", &cmd)
                    && config_setting_lookup_int(penButton, "input", &input)
                    && config_setting_lookup_int(penButton, "ctrl", &ctrl)
                    && config_setting_lookup_int(penButton, "alt", &alt)
                    && config_setting_lookup_int(penButton, "mode", &mode)
                    && config_setting_lookup_int(penButton, "key", &key)
                    && config_setting_lookup_int(penButton, "shift", &shift)))
                continue;

            penButtons[i].input = input;
            penButtons[i].mode = mode;
            penButtons[i].cmd = cmd;
            penButtons[i].alt = alt;
            penButtons[i].ctrl = ctrl;
            penButtons[i].shift = shift;
            penButtons[i].key = key;

        }
    }
//
    setting = config_lookup(&cfg, "tabletButtons");
    if(setting != NULL)
    {
        count = config_setting_length(setting);

        for(i = 0; i < count; ++i)
        {
            config_setting_t *penButton = config_setting_get_elem(setting, i);
//
//            /* Only output the record if all of the expected fields are present. */
//            const char *title, *media;
//            double price;
            int input, mode, cmd, ctrl, alt, shift, key;
//
            if(!(config_setting_lookup_int(penButton, "cmd", &cmd)
                    && config_setting_lookup_int(penButton, "input", &input)
                    && config_setting_lookup_int(penButton, "ctrl", &ctrl)
                    && config_setting_lookup_int(penButton, "alt", &alt)
                    && config_setting_lookup_int(penButton, "mode", &mode)
                    && config_setting_lookup_int(penButton, "key", &key)
                    && config_setting_lookup_int(penButton, "shift", &shift)))
                continue;

            tabletButtons[i].input = input;
            tabletButtons[i].mode = mode;
            tabletButtons[i].cmd = cmd;
            tabletButtons[i].alt = alt;
            tabletButtons[i].ctrl = ctrl;
            tabletButtons[i].shift = shift;
            tabletButtons[i].key = key;

        }
    }

    config_destroy(&cfg);

}