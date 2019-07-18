/*
Copyright (c) 2017-2018, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/


/***************************************************************
 *  Defines for Tool
 **************************************************************/
enum E_TAB_INDEX
{
    TAB_INDEX_NONE = -1,
    TAB_INDEX_SENSOR = 0,
    TAB_INDEX_EXPANSION,
    TAB_INDEX_AOIS,
    TAB_INDEX_EEPROM,
    TAB_INDEX_LAYOUT,
};

/*******************************************************************************
 * @brief   Read Setting File
 *
 * @param   filename        File name
 * @param   param           Parameters
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
static int ReadSettingFile(char* filename, ST_PARAM *param)
{
    FILE *fp;
    errno_t err;
    char buf[256];
    char itemName[256];
    char* pContext;
    E_TAB_INDEX select_tab_index = TAB_INDEX_NONE;

    err = fopen_s(&fp, filename, "r");
    if (err)
    {
        printf("%s file open failed.\r\n", filename);
        return 1;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // search "[Section]"
        if (strstr(buf, "[Sensor]"))
        {
            select_tab_index = TAB_INDEX_SENSOR;
            continue;
        }
        else if (strstr(buf, "[Expansion]"))
        {
            select_tab_index = TAB_INDEX_EXPANSION;
            continue;
        }
        else if (strstr(buf, "[EEPROM]"))
        {
            select_tab_index = TAB_INDEX_EEPROM;
            continue;
        }
        else if (strstr(buf, "[LAYOUT]"))
        {
            select_tab_index = TAB_INDEX_LAYOUT;
            continue;
        }
#ifdef CA378_AOIS
        else if (strstr(buf, "[AOIS]"))
        {
            select_tab_index = TAB_INDEX_AOIS;
            continue;
        }
#endif

        // select tab index
        switch (select_tab_index)
        {
        case TAB_INDEX_SENSOR:
            {
                char *p = strtok_s(buf, "= \r\n", &pContext);
                int n = 0;
                while (p)
                {
                    switch (n)
                    {
                    case 0:
                        strcpy_s(itemName, sizeof(itemName), p);
                        break;
                    case 1:
                        if (strcmp(itemName, "select_mode") == 0)
                        {
                            param->select_mode = atoi(p);
                        }
                        else if (strcmp(itemName, "record_frame") == 0)
                        {
                            param->record_frame = atoi(p);
                        }
                        break;
                    default:
                        break;
                    }
                    n++;
                    p = strtok_s(NULL, "= \r\n", &pContext);
                }
            }
            break;
        case TAB_INDEX_EXPANSION:
            {
                char *p = strtok_s(buf, "={}, \r\n", &pContext);
                int n = 0;
                while (p)
                {
                    switch (n)
                    {
                    case 0:
                        strcpy_s(itemName, sizeof(itemName), p);
                        break;
                    case 1:
                        if (strcmp(itemName, "enable_gamma") == 0)
                        {
                            param->enable_gamma = atoi(p);
                        }
                        else if (strcmp(itemName, "gamma_value") == 0)
                        {
                            param->gamma_value = atof(p);
                        }
                        if (strcmp(itemName, "enable_srgb_gamma") == 0)
                        {
                            param->enable_srgb_gamma = atoi(p);
                        }
                        if (strcmp(itemName, "enable_contrast") == 0)
                        {
                            param->enable_contrast = atoi(p);
                        }
                        else if (strcmp(itemName, "contrast_value") == 0)
                        {
                            param->contrast_value = atoi(p);
                        }
                        else if (strcmp(itemName, "enable_ccm") == 0)
                        {
                            param->enable_ccm = atoi(p);
                        }
                        else if (strcmp(itemName, "ccm") == 0)
                        {
                            param->ccm[0][0] = atof(p);
                        }
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        if (strcmp(itemName, "ccm") == 0)
                        {
                            int y = (n - 1) / 3;
                            int x = (n - 1) % 3;
                            param->ccm[y][x] = atof(p);
                        }
                        break;
                    default:
                        break;
                    }
                    n++;
                    p = strtok_s(NULL, "={}, \r\n", &pContext);
                }
            }
            break;
#ifdef CA378_AOIS
        case TAB_INDEX_AOIS:
            {
                char *p = strtok_s(buf, "= \r\n", &pContext);
                int n = 0;
                while (p)
                {
                    switch (n)
                    {
                    case 0:
                        strcpy_s(itemName, sizeof(itemName), p);
                        break;
                    case 1:
                        if (strcmp(itemName, "focus_mode") == 0)
                        {
                            param->focus_mode = atoi(p);
                        }
                        else if (strcmp(itemName, "manual_focus_position") == 0)
                        {
                            param->manual_focus_position = atoi(p);
                        }
                        else if (strcmp(itemName, "auto_focus_gain") == 0)
                        {
                            param->auto_focus_gain = atof(p);
                        }
                        break;
                    default:
                        break;
                    }
                    n++;
                    p = strtok_s(NULL, "= \r\n", &pContext);
                }
            }
            break;
#endif
        case TAB_INDEX_EEPROM:
            {
                char *p = strtok_s(buf, "=\r\n", &pContext);
                int n = 0;
                while (p)
                {
                    switch (n)
                    {
                    case 0:
                        strcpy_s(itemName, sizeof(itemName), p);
                        break;
                    case 1:
                        if (strcmp(itemName, "eeprom_table_number") == 0)
                        {
                            param->eeprom_table_number = atoi(p);
                        }
                        else if (strcmp(itemName, "lsc_table1_path") == 0)
                        {
                            param->lsc_table1_path = p;
                        }
                        else if (strcmp(itemName, "lsc_table2_path") == 0)
                        {
                            param->lsc_table2_path = p;
                        }
                        else if (strcmp(itemName, "lsc_table3_path") == 0)
                        {
                            param->lsc_table3_path = p;
                        }
                        break;
                    default:
                        break;
                    }
                    n++;
                    p = strtok_s(NULL, "=\r\n", &pContext);
                }
            }
            break;
        case TAB_INDEX_LAYOUT:
            {
                char *p = strtok_s(buf, "=\r\n", &pContext);
                int n = 0;
                while (p)
                {
                    switch (n)
                    {
                    case 0:
                        strcpy_s(itemName, sizeof(itemName), p);
                        break;
                    case 1:
                        if (strcmp(itemName, "font_family") == 0)
                        {
                            param->font_family = p;
                        }
                        else if (strcmp(itemName, "font_points") == 0)
                        {
                            param->font_points = atoi(p);
                        }
                        else if (strcmp(itemName, "scale") == 0)
                        {
                            param->scale = atoi(p);
                        }
                        else if (strcmp(itemName, "window_left") == 0)
                        {
                            param->window_left = atoi(p);
                        }
                        else if (strcmp(itemName, "window_top") == 0)
                        {
                            param->window_top = atoi(p);
                        }
                        else if (strcmp(itemName, "window_width") == 0)
                        {
                            param->window_width = atoi(p);
                        }
                        else if (strcmp(itemName, "window_height") == 0)
                        {
                            param->window_height = atoi(p);
                        }
                        break;
                    default:
                        break;
                    }
                    n++;
                    p = strtok_s(NULL, "=\r\n", &pContext);
                }
            }
            break;
        default:
            break;
        }
    }

    fclose(fp);

    return 0;
}

/*******************************************************************************
 * @brief   Write Setting File
 *
 * @param   filename        File name
 * @param   param           Parameters
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
static int WriteSettingFile(char* filename, ST_PARAM *param)
{
    FILE *fp;
    errno_t err;

    err = fopen_s(&fp, filename, "w");
    if (err)
    {
        printf("%s file open failed.\r\n", filename);
        return 1;
    }

    fprintf(fp, "[Sensor]\n");
    fprintf(fp, "select_mode=%d\n", param->select_mode);
    fprintf(fp, "record_frame=%d\n", param->record_frame);
    fprintf(fp, "[Expansion]\n");
    fprintf(fp, "enable_gamma=%d\n", param->enable_gamma);
    fprintf(fp, "gamma_value=%.3lf\n", param->gamma_value);
    fprintf(fp, "enable_srgb_gamma=%d\n", param->enable_srgb_gamma);
    fprintf(fp, "enable_contrast=%d\n", param->enable_contrast);
    fprintf(fp, "contrast_value=%d\n", param->contrast_value);
    fprintf(fp, "enable_ccm=%d\n", param->enable_ccm);
    fprintf(fp, "ccm={%.3lf,%.3lf,%.3lf},{%.3lf,%.3lf,%.3lf},{%.3lf,%.3lf,%.3lf}\n",
        param->ccm[0][0], param->ccm[0][1], param->ccm[0][2],
        param->ccm[1][0], param->ccm[1][1], param->ccm[1][2],
        param->ccm[2][0], param->ccm[2][1], param->ccm[2][2]);
#ifdef CA378_AOIS
    fprintf(fp, "[AOIS]\n");
    fprintf(fp, "focus_mode=%d\n", param->focus_mode);
    fprintf(fp, "manual_focus_position=%d\n", param->manual_focus_position);
    fprintf(fp, "auto_focus_gain=%.1lf\n", param->auto_focus_gain);
#endif
    fprintf(fp, "[EEPROM]\n");
    fprintf(fp, "eeprom_table_number=%d\n", param->eeprom_table_number);
    fprintf(fp, "lsc_table1_path=%s\n", param->lsc_table1_path.c_str());
    fprintf(fp, "lsc_table2_path=%s\n", param->lsc_table2_path.c_str());
    fprintf(fp, "lsc_table3_path=%s\n", param->lsc_table3_path.c_str());
    fprintf(fp, "[LAYOUT]\n");
    fprintf(fp, "font_family=%s\n", param->font_family.c_str());
    fprintf(fp, "font_points=%d\n", param->font_points);
    fprintf(fp, "scale=%d\n", param->scale);
    fprintf(fp, "window_left=%d\n", param->window_left);
    fprintf(fp, "window_top=%d\n", param->window_top);
    fprintf(fp, "window_width=%d\n", param->window_width);
    fprintf(fp, "window_height=%d\n", param->window_height);

    fclose(fp);

    return 0;
}

/*******************************************************************************
 * @brief   Load LSC (Lens Shading Correction) Table
 *
 * @param   filename        File name
 * @param   bayer           bayer index (0:R, 1:GR, 2:GB, 3:B)
 * @param   data            LSC data
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
static int LoadLensShadingTable(char* filename, uint8_t bayer, uint8_t* data)
{
    FILE *fp;
    errno_t err;
    char buf[256];
    char search_bayer[][4] = { "#R", "#GR", "#GB", "#B" };
    char* pContext;
    uint16_t value;
    int index = 0;
    int select = 0;

    err = fopen_s(&fp, filename, "r");
    if (err)
    {
        printf("%s file open failed.\r\n", filename);
        return 1;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        // search "#"
        if (strstr(buf, "#"))
        {
            // search bayer ID
            if (strstr(buf, search_bayer[bayer]))
            {
                select = 1;
                continue;
            }
            else
            {
                select = 0;
                continue;
            }
        }

        // select bayer
        if (select)
        {
            char *p = strtok_s(buf, ", \r\n", &pContext);
            int n = 0;
            while (p)
            {
                if (n < 8)
                {
                    // load shading data
                    if (index < 96)
                    {
                        value = atoi(p);
                        data[index + 0] = (value >> 8) & 0xFF;
                        data[index + 1] = (value     ) & 0xFF;
                        index += 2;
                    }
                }
                n++;
                p = strtok_s(NULL, ", \r\n", &pContext);
            }
        }
    }

    fclose(fp);

    return 0;
}

/*******************************************************************************
 * @brief   Save LSC (Lens Shading Correction) Table
 *
 * @param   filename        File name
 * @param   bayer           bayer index (0:R, 1:GR, 2:GB, 3:B)
 * @param   data            LSC data
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
static int SaveLensShadingTable(char* filename, uint8_t bayer, uint8_t* data)
{
    FILE *fp;
    errno_t err;
    char search_bayer[][4] = { "#R", "#GR", "#GB", "#B" };
    uint16_t value;
    int index = 0;

    if (bayer == 0)
    {
        err = fopen_s(&fp, filename, "w");
    }
    else
    {
        err = fopen_s(&fp, filename, "a");
    }

    if (err)
    {
        printf("%s file open failed.\r\n", filename);
        return 1;
    }

    fprintf(fp, "%s\n", search_bayer[bayer]);
    for (index = 0; index < 96; index += 2)
    {
        value = (data[index] << 8) + data[index + 1];
        if ((index / 2) % 8 == 7)
        {
            fprintf(fp, "%d\n", value);
        }
        else
        {
            fprintf(fp, "%d,", value);
        }
    }
    fprintf(fp, "\n");

    fclose(fp);

    return 0;
}
