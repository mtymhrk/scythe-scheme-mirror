#ifndef INCLUDE_NUMBER_PARSER_H__
#define INCLUDE_NUMBER_PARSER_H__

#include <sys/types.h>
#include <stddef.h>

#include "scythe/encoding.h"
#include "scythe/earray.h"

typedef struct ScmNumParseIntDecDataRec ScmNumParseIntDecData;
typedef struct ScmNumParseRatDataRec ScmNumParseRatData;
typedef struct ScmNumParseRealDataRec ScmNumParseRealData;
typedef struct ScmNumParseDataRec ScmNumParseData;

typedef enum scm_num_parse_rslt scm_num_parse_rslt_t;

enum scm_num_parse_rslt {
  SCM_NUM_PARSE_SUCCESS,
  SCM_NUM_PARSE_INVALID,
  SCM_NUM_PARSE_INTERNAL_ERR,
};

struct ScmNumParseIntDecDataRec {
  size_t head;                  /* 先頭数字 index                           */
  size_t len;                   /* 数字文字数 (小数点含む、suffix 含まず)   */
  ssize_t point;                /* 小数点 index (負数の場合、小数点無し)    */
  char s_sign;                  /* +, -, \0  (suffix 符号, \0 の場合 suffix
                                 * 無し)                                    */
  size_t s_head;                /* suffix 先頭数字 index                    */
  size_t s_len;                 /* suffix 数字文字数                        */
};

struct ScmNumParseRatDataRec {
  ScmNumParseIntDecData num;           /* 分子 */
  ScmNumParseIntDecData den;           /* 分母 */
};

struct ScmNumParseRealDataRec {
  char sign;                    /* +, -, '\0' */
  char type;                    /* r: 有理数,
                                 * i: 整数or少数。情報は rat.num に保持
                                 * I: inf.0
                                 * N: nan.0
                                 * \0: rat に情報無し */
  ScmNumParseRatData rat;
};

struct ScmNumParseDataRec {
  scm_num_parse_rslt_t rslt;
  char radix;                   /* b: 2 進数, o: 8進数, d: 10進数, x: 16 進数 */
  char exact;                   /* i: inexact, e: exact, \0: 指定無し */
  char complex;                 /* @: 極形式,
                                 * o: 直交形式(虚数無し),
                                 * O: 直交形式(虚数有り)
                                 *    (実数の有無は fir.type により判断する) */
  ScmNumParseRealData fir;
  ScmNumParseRealData sec;
};


ScmNumParseData *scm_num_parse(ScmObj port, EArray *str, ScmNumParseData *data);

ScmObj scm_num_make_from_parsedata(const scm_char_t *str, ScmEncoding *enc,
                                   const ScmNumParseData *data);
ScmObj scm_num_make_from_literal(const char *literal, ScmEncoding *enc);

#endif /* INCLUDE_NUMBER_PARSER_H__ */
