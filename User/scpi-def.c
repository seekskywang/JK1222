/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser test
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scpi/scpi.h"
#include "scpi-def.h"
//#include "pbdata.h"

uint8_t remotedispflag;
uint8_t remoteonoffflag;
uint8_t scpirecflag;
uint8_t scpirectimeout;
scpi_choice_def_t mode_source[] = {
    {"CCH", 0},
		{"CCL", 0},
    {"CVH", 1},
		{"CVL", 1},
    {"CP", 3},
		{"CRH", 2},
		{"CRM", 2},
		{"CRL", 2},
    {"DYNamic", 4},
    {"LED", 5},
		{"SHORt", 6},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

scpi_choice_def_t input_source[] = {
    {"OFF", 0},
    {"ON", 1},
    SCPI_CHOICE_LIST_END /* termination of option list */
};
static const char CurPage[][10]={
	{"MEAS"},//测量显示
    {"LIST"},//列表显示
    {"MSET"},//测量设置
    {"MCON"},//测量配置
};

static const char IRUNIT[][3]={
	{"k"},//
    {"M"},//
    {"G"},//
};

static const char Respond[][5]={
	{"OK"}
};
char Resbuf[500];

extern void InputSwitch(u8 para);


static scpi_result_t DMM_MeasureVoltageAcQ(scpi_t * context) {
    scpi_number_t param1, param2;
    char bf[15];
//    fprintf(stderr, "meas:volt:ac\r\n"); /* debug command name */

    /* read first parameter if present */
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param1, FALSE)) {
        /* do something, if parameter not present */
    }

    /* read second paraeter if present */
    if (!SCPI_ParamNumber(context, scpi_special_numbers_def, &param2, FALSE)) {
        /* do something, if parameter not present */
    }


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param1, bf, 15);
//    fprintf(stderr, "\tP1=%s\r\n", bf);


    SCPI_NumberToStr(context, scpi_special_numbers_def, &param2, bf, 15);
//    fprintf(stderr, "\tP2=%s\r\n", bf);

    SCPI_ResultDouble(context, 0);

    return SCPI_RES_OK;
}

static scpi_result_t DMM_ConfigureVoltageDc(scpi_t * context) {
    double param1, param2;
//    fprintf(stderr, "conf:volt:dc\r\n"); /* debug command name */

    /* read first parameter if present */
    if (!SCPI_ParamDouble(context, &param1, TRUE)) {
        return SCPI_RES_ERR;
    }

    /* read second paraeter if present */
    if (!SCPI_ParamDouble(context, &param2, FALSE)) {
        /* do something, if parameter not present */
    }

//    fprintf(stderr, "\tP1=%lf\r\n", param1);
//    fprintf(stderr, "\tP2=%lf\r\n", param2);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Bool(scpi_t * context) {
    scpi_bool_t param1;
//    fprintf(stderr, "TEST:BOOL\r\n"); /* debug command name */

    /* read first parameter if present */
    if (!SCPI_ParamBool(context, &param1, TRUE)) {
        return SCPI_RES_ERR;
    }

//    fprintf(stderr, "\tP1=%d\r\n", param1);

    return SCPI_RES_OK;
}

scpi_choice_def_t trigger_source[] = {
    {"BUS", 5},
    {"IMMediate", 6},
    {"EXTernal", 7},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

static scpi_result_t TEST_ChoiceQ(scpi_t * context) {

    int32_t param;
    const char * name;

    if (!SCPI_ParamChoice(context, trigger_source, &param, TRUE)) {
        return SCPI_RES_ERR;
    }

    SCPI_ChoiceToName(trigger_source, param, &name);
//    fprintf(stderr, "\tP1=%s (%ld)\r\n", name, (long int) param);

    SCPI_ResultInt32(context, param);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Numbers(scpi_t * context) {
    int32_t numbers[2];

    SCPI_CommandNumbers(context, numbers, 2, 1);

//    fprintf(stderr, "TEST numbers %d %d\r\n", numbers[0], numbers[1]);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_Text(scpi_t * context) {
    char buffer[100];
    size_t copy_len;

    if (!SCPI_ParamCopyText(context, buffer, sizeof (buffer), &copy_len, FALSE)) {
        buffer[0] = '\0';
    }

//    fprintf(stderr, "TEXT: ***%s***\r\n", buffer);

    return SCPI_RES_OK;
}

static scpi_result_t TEST_ArbQ(scpi_t * context) {
    const char * data;
    size_t len;

    if (SCPI_ParamArbitraryBlock(context, &data, &len, FALSE)) {
        SCPI_ResultArbitraryBlock(context, data, len);
    }

    return SCPI_RES_OK;
}

struct _scpi_channel_value_t {
    int32_t row;
    int32_t col;
};
typedef struct _scpi_channel_value_t scpi_channel_value_t;

/**
 * @brief
 * parses lists
 * channel numbers > 0.
 * no checks yet.
 * valid: (@1), (@3!1:1!3), ...
 * (@1!1:3!2) would be 1!1, 1!2, 2!1, 2!2, 3!1, 3!2.
 * (@3!1:1!3) would be 3!1, 3!2, 3!3, 2!1, 2!2, 2!3, ... 1!3.
 *
 * @param channel_list channel list, compare to SCPI99 Vol 1 Ch. 8.3.2
 */
static scpi_result_t TEST_Chanlst(scpi_t *context) {
    scpi_parameter_t channel_list_param;
#define MAXROW 2    /* maximum number of rows */
#define MAXCOL 6    /* maximum number of columns */
#define MAXDIM 2    /* maximum number of dimensions */
    scpi_channel_value_t array[MAXROW * MAXCOL]; /* array which holds values in order (2D) */
    size_t chanlst_idx; /* index for channel list */
    size_t arr_idx = 0; /* index for array */
    size_t n, m = 1; /* counters for row (n) and columns (m) */

    /* get channel list */
    if (SCPI_Parameter(context, &channel_list_param, TRUE)) {
        scpi_expr_result_t res;
        scpi_bool_t is_range;
        int32_t values_from[MAXDIM];
        int32_t values_to[MAXDIM];
        size_t dimensions;

        bool for_stop_row = FALSE; /* true if iteration for rows has to stop */
        bool for_stop_col = FALSE; /* true if iteration for columns has to stop */
        int32_t dir_row = 1; /* direction of counter for rows, +/-1 */
        int32_t dir_col = 1; /* direction of counter for columns, +/-1 */

        /* the next statement is valid usage and it gets only real number of dimensions for the first item (index 0) */
        if (!SCPI_ExprChannelListEntry(context, &channel_list_param, 0, &is_range, NULL, NULL, 0, &dimensions)) {
            chanlst_idx = 0; /* call first index */
            arr_idx = 0; /* set arr_idx to 0 */
            do { /* if valid, iterate over channel_list_param index while res == valid (do-while cause we have to do it once) */
                res = SCPI_ExprChannelListEntry(context, &channel_list_param, chanlst_idx, &is_range, values_from, values_to, 4, &dimensions);
                if (is_range == FALSE) { /* still can have multiple dimensions */
                    if (dimensions == 1) {
                        /* here we have our values
                         * row == values_from[0]
                         * col == 0 (fixed number)
                         * call a function or something */
                        array[arr_idx].row = values_from[0];
                        array[arr_idx].col = 0;
                    } else if (dimensions == 2) {
                        /* here we have our values
                         * row == values_fom[0]
                         * col == values_from[1]
                         * call a function or something */
                        array[arr_idx].row = values_from[0];
                        array[arr_idx].col = values_from[1];
                    } else {
                        return SCPI_RES_ERR;
                    }
                    arr_idx++; /* inkrement array where we want to save our values to, not neccessary otherwise */
                    if (arr_idx >= MAXROW * MAXCOL) {
                        return SCPI_RES_ERR;
                    }
                } else if (is_range == TRUE) {
                    if (values_from[0] > values_to[0]) {
                        dir_row = -1; /* we have to decrement from values_from */
                    } else { /* if (values_from[0] < values_to[0]) */
                        dir_row = +1; /* default, we increment from values_from */
                    }

                    /* iterating over rows, do it once -> set for_stop_row = false
                     * needed if there is channel list index isn't at end yet */
                    for_stop_row = FALSE;
                    for (n = values_from[0]; for_stop_row == FALSE; n += dir_row) {
                        /* usual case for ranges, 2 dimensions */
                        if (dimensions == 2) {
                            if (values_from[1] > values_to[1]) {
                                dir_col = -1;
                            } else if (values_from[1] < values_to[1]) {
                                dir_col = +1;
                            }
                            /* iterating over columns, do it at least once -> set for_stop_col = false
                             * needed if there is channel list index isn't at end yet */
                            for_stop_col = FALSE;
                            for (m = values_from[1]; for_stop_col == FALSE; m += dir_col) {
                                /* here we have our values
                                 * row == n
                                 * col == m
                                 * call a function or something */
                                array[arr_idx].row = n;
                                array[arr_idx].col = m;
                                arr_idx++;
                                if (arr_idx >= MAXROW * MAXCOL) {
                                    return SCPI_RES_ERR;
                                }
                                if (m == (size_t)values_to[1]) {
                                    /* endpoint reached, stop column for-loop */
                                    for_stop_col = TRUE;
                                }
                            }
                            /* special case for range, example: (@2!1) */
                        } else if (dimensions == 1) {
                            /* here we have values
                             * row == n
                             * col == 0 (fixed number)
                             * call function or sth. */
                            array[arr_idx].row = n;
                            array[arr_idx].col = 0;
                            arr_idx++;
                            if (arr_idx >= MAXROW * MAXCOL) {
                                return SCPI_RES_ERR;
                            }
                        }
                        if (n == (size_t)values_to[0]) {
                            /* endpoint reached, stop row for-loop */
                            for_stop_row = TRUE;
                        }
                    }


                } else {
                    return SCPI_RES_ERR;
                }
                /* increase index */
                chanlst_idx++;
            } while (SCPI_EXPR_OK == SCPI_ExprChannelListEntry(context, &channel_list_param, chanlst_idx, &is_range, values_from, values_to, 4, &dimensions));
            /* while checks, whether incremented index is valid */
        }
        /* do something at the end if needed */
        /* array[arr_idx].row = 0; */
        /* array[arr_idx].col = 0; */
    }

    {
        size_t i;
//        fprintf(stderr, "TEST_Chanlst: ");
        for (i = 0; i< arr_idx; i++) {
//            fprintf(stderr, "%d!%d, ", array[i].row, array[i].col);
        }
//        fprintf(stderr, "\r\n");
    }
    return SCPI_RES_OK;
}

/**
 * Reimplement IEEE488.2 *TST?
 *
 * Result should be 0 if everything is ok
 * Result should be 1 if something goes wrong
 *
 * Return SCPI_RES_OK
 */
static scpi_result_t My_CoreTstQ(scpi_t * context) {

    SCPI_ResultInt32(context, 8);

    return SCPI_RES_OK;
}

//进入本地模式
static scpi_result_t LOCAL(scpi_t * context) {

//	Operation_MODE=0;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//进入远程模式
static scpi_result_t REMOTE(scpi_t * context) {

//	Operation_MODE=1;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//进入远程模式,所有按键都禁止
static scpi_result_t RWLOCK(scpi_t * context) {

//	Operation_MODE=2;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//设置远端测量
static scpi_result_t REMOTESET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	uint32_t param;

	if (!SCPI_ParamUInt32(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	Sence_SW = param;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询远端测量
static scpi_result_t REMOTEQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,Sence_SW);

  return SCPI_RES_OK;
}

//控制输入开关
static scpi_result_t INPUTSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	int32_t param;

	if (!SCPI_ParamChoice(context, input_source, &param, TRUE)) {
			return SCPI_RES_ERR;
	}
	mainswitch = param;
	scpirecflag = 1;
	scpirectimeout = 0;
//	remoteonoffflag = 1;
//	InputSwitch(param);
//	onoff_ch = param;
//	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询输入开关
static scpi_result_t INPUTQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


  SCPI_ResultFloat(context,1);

  return SCPI_RES_OK;
}

//设置电压档位
static scpi_result_t VRANGESET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	uint32_t param;

	if (!SCPI_ParamUInt32(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	V_Gear_SW = param;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询电压档位
static scpi_result_t VRANGEQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,V_Gear_SW);

  return SCPI_RES_OK;
}

//设置电流档位
static scpi_result_t IRANGESET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	uint32_t param;

	if (!SCPI_ParamUInt32(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	I_Gear_SW = param;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询电流档位
static scpi_result_t IRANGEQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,I_Gear_SW);

  return SCPI_RES_OK;
}

//设置相同的电流上升下降率
static scpi_result_t ISLEWSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	I_Rise_Time = param*10000;
//	I_Down_Time = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询相同的电流上升下降率
static scpi_result_t ISLEWQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)I_Rise_Time/10000);

  return SCPI_RES_OK;
}

//设置电流上升率
static scpi_result_t ISLEWRSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	I_Rise_Time = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询电流上升率
static scpi_result_t ISLEWRQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)I_Rise_Time/10000);

  return SCPI_RES_OK;
}

//设置电流下降率
static scpi_result_t ISLEWFSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	I_Down_Time = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询电流下降率
static scpi_result_t ISLEWFQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)I_Down_Time/10000);

  return SCPI_RES_OK;
}

//设置保护电流
static scpi_result_t CPROTECTSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	MAX_I = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询保护电流
static scpi_result_t CPROTECTQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)MAX_I/1000);

  return SCPI_RES_OK;
}

//设置保护功率
static scpi_result_t PPROTECTSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	MAX_P = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询保护功率
static scpi_result_t PPROTECTQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)MAX_P/1000);

  return SCPI_RES_OK;
}

//设置保护电压
static scpi_result_t VPROTECTSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	MAX_V = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询保护电压
static scpi_result_t VPROTECTQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)MAX_V/1000);

  return SCPI_RES_OK;
}

//设置启动电压
static scpi_result_t VONSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	VON_Voltage = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询启动电压
static scpi_result_t VONQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)VON_Voltage/1000);

  return SCPI_RES_OK;
}

//设置卸载电压
static scpi_result_t VOFFSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	VOFF_Voltage = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询卸载电压
static scpi_result_t VOFFQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)VOFF_Voltage/1000);

  return SCPI_RES_OK;
}

//设置电压下降率
static scpi_result_t VSLEWSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	CV_Down_Time = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询电压下降率
static scpi_result_t VSLEWQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)CV_Down_Time/10000);

  return SCPI_RES_OK;
}



//设置工作模式
static scpi_result_t MODESET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);
	int32_t param;
	

	if (!SCPI_ParamChoice(context, mode_source, &param, TRUE)) {
			return SCPI_RES_ERR;
	}

//	SCPI_ChoiceToName(mode_source, param, &name);
//	MODE=param;
	LoadSave.mode = param;
	scpirecflag = 1;
	scpirectimeout = 0;
//	Mode_SW();
//	Store_set_flash();
	remotedispflag = 1;
//    fprintf(stderr, "\tP1=%s (%ld)\r\n", name, (long int) param);
//	SCPI_ResultCharacters(context, Respond[0],2);
//	SCPI_ResultInt32(context, param);

	return SCPI_RES_OK;
}

//查询工作模式
static scpi_result_t MODEQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);
	const char * name;

//	SCPI_ChoiceToName(mode_source, MODE, &name);
  SCPI_ResultCharacters(context,name,sizeof(name));

  return SCPI_RES_OK;
}

//设置CC电流
static scpi_result_t CCSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	SET_Current = param*10000;
	LoadSave.current = param*10000;
	scpirecflag = 1;
	scpirectimeout = 0;
//	Para_Set_Comp();
//	Set_Para();
	remotedispflag = 1;
//	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询CC电流
static scpi_result_t CCQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)SET_Current/10000);

  return SCPI_RES_OK;
}

//设置CV电压
static scpi_result_t CVSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	SET_Voltage = param*10000;
	LoadSave.voltage = param*10000;
	scpirecflag = 1;
	scpirectimeout = 0;
//	Para_Set_Comp();
//	Set_Para();
	remotedispflag = 1;
//	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询CV电压
static scpi_result_t CVQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)SET_Voltage/10000);

  return SCPI_RES_OK;
}

//设置CP功率
static scpi_result_t CPSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	SET_Power = param*1000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询CP功率
static scpi_result_t CPQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)SET_Power/1000);

  return SCPI_RES_OK;
}

//设置CR电阻
static scpi_result_t CRSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
	LoadSave.risistence = param*10;
	scpirecflag = 1;
	scpirectimeout = 0;
//	Para_Set_Comp();
//	Set_Para();
	remotedispflag = 1;
//	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询CR电阻
static scpi_result_t CRQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)SET_Resist/1000);

  return SCPI_RES_OK;
}

//设置动态拉载电流A
static scpi_result_t DYNHSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_Ia = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态拉载电流A
static scpi_result_t DYNHQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_Ia/10000);

  return SCPI_RES_OK;
}

//设置动态拉载时间A 单位0.1ms
static scpi_result_t DYNHDSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_Ta = param*10;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态拉载时间A 单位0.1ms
static scpi_result_t DYNHDQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_Ta/10);

  return SCPI_RES_OK;
}

//设置动态拉载电流B
static scpi_result_t DYNLSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_Ib = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态拉载电流B
static scpi_result_t DYNLQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_Ib/10000);

  return SCPI_RES_OK;
}

//设置动态拉载时间B 单位0.1ms
static scpi_result_t DYNLDSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_Tb = param*10;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态拉载时间B 单位0.1ms
static scpi_result_t DYNLDQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_Tb/10);

  return SCPI_RES_OK;
}

//设置相同的电流上升下降率
static scpi_result_t DYNSLEWSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_IRise = param*10000;
//	DYNA_IDown = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询相同的动态电流上升下降率
static scpi_result_t DYNSLEWQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_IRise/10000);

  return SCPI_RES_OK;
}

//设置动态电流上升率
static scpi_result_t DYNSLEWRSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_IRise = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态电流上升率
static scpi_result_t DYNSLEWRQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_IRise/10000);

  return SCPI_RES_OK;
}

//设置动态电流下降率
static scpi_result_t DYNSLEWFSET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);

	float param;

	if (!SCPI_ParamFloat(context,&param, FALSE)) {
		return SCPI_RES_ERR;
	}
//	DYNA_IDown = param*10000;
	SCPI_ResultCharacters(context, Respond[0],2);
  return SCPI_RES_OK;
}

//查询动态电流下降率
static scpi_result_t DYNSLEWFQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);


//  SCPI_ResultFloat(context,(float)DYNA_IDown/10000);

  return SCPI_RES_OK;
}

scpi_choice_def_t dynmode_source[] = {
    {"CONTinuous", 0},
    {"PULSe", 1},
    {"TOGGle", 2},
    SCPI_CHOICE_LIST_END /* termination of option list */
};

//设置动态工作模式
static scpi_result_t DYNMODESET(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);
	int32_t param;
	

	if (!SCPI_ParamChoice(context, dynmode_source, &param, TRUE)) {
			return SCPI_RES_ERR;
	}

//	SCPI_ChoiceToName(mode_source, param, &name);
//	DYNA_MODE=param;
//    fprintf(stderr, "\tP1=%s (%ld)\r\n", name, (long int) param);
	SCPI_ResultCharacters(context, Respond[0],2);
//	SCPI_ResultInt32(context, param);

	return SCPI_RES_OK;
}

//查询动态工作模式
static scpi_result_t DYNMODEQ(scpi_t * context) {
//    fprintf(stderr, "\tP2=%s\r\n", bf);
	const char * name;

//	SCPI_ChoiceToName(dynmode_source, DYNA_MODE, &name);
  SCPI_ResultCharacters(context,name,sizeof(name));

  return SCPI_RES_OK;
}

//查询电压
static scpi_result_t VOLTAGEQ(scpi_t * context) {

	if(DispValue.Voltage==0)
	{
		SCPI_ResultFloat(context,((float)DispValue.Voltage)/10000);
	}else{
		SCPI_ResultFloat(context,((float)DispValue.Voltage)/1000);
	}
  return SCPI_RES_OK;

}

//查询电流
static scpi_result_t CURRENTQ(scpi_t * context) {

	if(LoadSave.crange==0)
	{
		SCPI_ResultFloat(context,((float)DispValue.Current)/10000);
	}else{
		SCPI_ResultFloat(context,((float)DispValue.Current)/1000);
	}

  return SCPI_RES_OK;

}

//查询功率
static scpi_result_t POWERQ(scpi_t * context) {

//	SCPI_ResultFloat(context,(float)Power_DATE/1000);

  return SCPI_RES_OK;

}

//查询阻抗
static scpi_result_t RESQ(scpi_t * context) {

//	SCPI_ResultFloat(context,(float)R_DATE/1000);

  return SCPI_RES_OK;

}
const scpi_command_t scpi_commands[] = {
	/* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
	{ .pattern = "*CLS", .callback = SCPI_CoreCls,},
	{ .pattern = "*ESE", .callback = SCPI_CoreEse,},
	{ .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
	{ .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
	{ .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
	{ .pattern = "*OPC", .callback = SCPI_CoreOpc,},
	{ .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
	{ .pattern = "*RST", .callback = SCPI_CoreRst,},
	{ .pattern = "*SRE", .callback = SCPI_CoreSre,},
	{ .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
	{ .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
	{ .pattern = "*TST?", .callback = My_CoreTstQ,},
	{ .pattern = "*WAI", .callback = SCPI_CoreWai,},

	/* Required SCPI commands (SCPI std V1999.0 4.2.1) */
	{.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorNextQ,},
	{.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
	{.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},

	/* {.pattern = "STATus:OPERation?", .callback = scpi_stub_callback,}, */
	/* {.pattern = "STATus:OPERation:EVENt?", .callback = scpi_stub_callback,}, */
	/* {.pattern = "STATus:OPERation:CONDition?", .callba	ck = scpi_stub_callback,}, */
	/* {.pattern = "STATus:OPERation:ENABle", .callback = scpi_stub_callback,}, */
	/* {.pattern = "STATus:OPERation:ENABle?", .callback = scpi_stub_callback,}, */

	{.pattern = "STATus:QUEStionable[:EVENt]?", .callback = SCPI_StatusQuestionableEventQ,},
	/* {.pattern = "STATus:QUEStionable:CONDition?", .callback = scpi_stub_callback,}, */
	{.pattern = "STATus:QUEStionable:ENABle", .callback = SCPI_StatusQuestionableEnable,},
	{.pattern = "STATus:QUEStionable:ENABle?", .callback = SCPI_StatusQuestionableEnableQ,},

	{.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,},

	/* DMM */
	{.pattern = "CONFigure:VOLTage:DC", .callback = DMM_ConfigureVoltageDc,},
	{.pattern = "MEASure:VOLTage:DC:RATio?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:VOLTage:AC?", .callback = DMM_MeasureVoltageAcQ,},
	{.pattern = "MEASure:CURRent:DC?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:CURRent:AC?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:RESistance?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:FRESistance?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:FREQuency?", .callback = SCPI_StubQ,},
	{.pattern = "MEASure:PERiod?", .callback = SCPI_StubQ,},


	{.pattern = "TEST:BOOL", .callback = TEST_Bool,},
	{.pattern = "TEST:CHOice?", .callback = TEST_ChoiceQ,},
	{.pattern = "TEST#:NUMbers#", .callback = TEST_Numbers,},
	{.pattern = "TEST:TEXT", .callback = TEST_Text,},
	{.pattern = "TEST:ARBitrary?", .callback = TEST_ArbQ,},
	{.pattern = "TEST:CHANnellist", .callback = TEST_Chanlst,},
	/*TEST*/
	{.pattern = "SYSTem:LOCal", .callback = REMOTESET,},
	{.pattern = "SYSTem:REMote", .callback = REMOTEQ,},
	{.pattern = "SYSTem:RWLock", .callback = REMOTEQ,},
	{.pattern = "SYSTem:SENSe", .callback = REMOTESET,},
	{.pattern = "SYSTem:SENSe?", .callback = REMOTEQ,},
	{.pattern = "INPut", .callback = INPUTSET,},
	{.pattern = "INPut?", .callback = INPUTQ,},
	{.pattern = "VOLTage:RANGe", .callback = VRANGESET,},
	{.pattern = "VOLTage:RANGe?", .callback = VRANGEQ,},
	{.pattern = "CURRent:RANGe", .callback = IRANGESET,},
	{.pattern = "CURRent:RANGe?", .callback = IRANGEQ,},
	{.pattern = "CURRent:SLEW", .callback = ISLEWSET,},
	{.pattern = "CURRent:SLEW?", .callback = ISLEWQ,},
	{.pattern = "CURRent:SLEW:RISE", .callback = ISLEWRSET,},
	{.pattern = "CURRent:SLEW:RISE?", .callback = ISLEWRQ,},
	{.pattern = "CURRent:SLEW:FALL", .callback = ISLEWFSET,},
	{.pattern = "CURRent:SLEW:FALL?", .callback = ISLEWFQ,},
	{.pattern = "CURRent:PROTection", .callback = CPROTECTSET,},
	{.pattern = "CURRent:PROTection?", .callback = CPROTECTQ,},
	{.pattern = "VOLTage:PROTection", .callback = VPROTECTSET,},
	{.pattern = "VOLTage:PROTection?", .callback = VPROTECTQ,},
	{.pattern = "POWer:PROTection", .callback = PPROTECTSET,},
	{.pattern = "POWer:PROTection?", .callback = PPROTECTQ,},
	{.pattern = "VOLTage:ON", .callback = VONSET,},
	{.pattern = "VOLTage:ON?", .callback = VONQ,},
	{.pattern = "VOLTage:OFF", .callback = VOFFSET,},
	{.pattern = "VOLTage:OFF?", .callback = VOFFQ,},
	{.pattern = "VOLTage:SLEW", .callback = VOFFSET,},
	{.pattern = "VOLTage:SLEW?", .callback = VOFFQ,},
	{.pattern = "FUNCtion", .callback = MODESET,},
	{.pattern = "FUNCtion?", .callback = MODEQ,},
	{.pattern = "MODE", .callback = MODESET,},
	{.pattern = "MODE?", .callback = MODEQ,},
	{.pattern = "CURRent", .callback = CCSET,},
	{.pattern = "CURRent?", .callback = CCQ,},
	{.pattern = "VOLTage", .callback = CVSET,},
	{.pattern = "VOLTage?", .callback = CVQ,},
	{.pattern = "POWer", .callback = CPSET,},
	{.pattern = "POWer?", .callback = CPQ,},
	{.pattern = "RESistance", .callback = CRSET,},
	{.pattern = "RESistance?", .callback = CRQ,},
	{.pattern = "DYNamic:HIGH", .callback = DYNHSET,},
	{.pattern = "DYNamic:HIGH?", .callback = DYNHQ,},
	{.pattern = "DYNamic:HIGH:DWELl", .callback = DYNHDSET,},
	{.pattern = "DYNamic:HIGH:DWELl?", .callback = DYNHDQ,},
	{.pattern = "DYNamic:LOW", .callback = DYNLSET,},
	{.pattern = "DYNamic:LOW?", .callback = DYNLQ,},
	{.pattern = "DYNamic:LOW:DWELl", .callback = DYNLDSET,},
	{.pattern = "DYNamic:LOW:DWELl?", .callback = DYNLDQ,},
	{.pattern = "DYNamic:SLEW", .callback = DYNSLEWSET,},
	{.pattern = "DYNamic:SLEW?", .callback = DYNSLEWQ,},
	{.pattern = "DYNamic:SLEW:RISE", .callback = DYNSLEWRSET,},
	{.pattern = "DYNamic:SLEW:RISE?", .callback = DYNSLEWRQ,},
	{.pattern = "DYNamic:SLEW:FALL", .callback = DYNSLEWFSET,},
	{.pattern = "DYNamic:SLEW:FALL?", .callback = DYNSLEWFQ,},
	{.pattern = "DYNamic:MODE", .callback = DYNMODESET,},
	{.pattern = "DYNamic:MODE?", .callback = DYNMODEQ,},
	{.pattern = "MEASure:VOLTage?", .callback = VOLTAGEQ,},
	{.pattern = "MEASure:CURRent?", .callback = CURRENTQ,},
	{.pattern = "MEASure:POWer?", .callback = POWERQ,},
	{.pattern = "MEASure:RESistance?", .callback = RESQ,},
    SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = {
    .error = NULL,
    .write = SCPI_Write, 
    .control = NULL,
    .flush = NULL,
    .reset = NULL, 
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;
