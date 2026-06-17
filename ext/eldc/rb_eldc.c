/*
 * Copyright 2026 gemmaro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rb_eldc.h"
#include <eldc_lib.h>
#include <ruby.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

VALUE rb_mELDC, rb_cDetector, rb_cDetectResult, rb_cScoreItem;

/* detector */

static void
rb_eldc_detector_free (void *detector)
{
  (void)detector;
  eldc_close ();
}

static const rb_data_type_t rb_eldc_detector_type = {
  .wrap_struct_name = "ELDC detector",
  .function = { .dfree = rb_eldc_detector_free },
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
rb_eldc_detector_alloc (VALUE klass)
{
  return rb_data_typed_object_wrap (rb_cDetector, NULL,
                                    &rb_eldc_detector_type);
}

/**
 * :nodoc:
 */
static VALUE
rb_eldc_detector_m_initialize (VALUE self)
{
  eldc_init ();
  return RUBY_Qnil;
}

static VALUE
rb_eldc_make_language_value (const char *language)
{
  return strcmp (language, "und") ? rb_str_new_cstr (language) : RUBY_Qnil;
}

/**
 * :call-seq:
 *   detect ("TEXT") -> "LANG"
 */
static VALUE
rb_eldc_detector_m_detect (VALUE self, VALUE text)
{
  const char *text_cstr = rb_string_value_cstr (&text);
  const char *language = eldc_detect (text_cstr);
  return rb_eldc_make_language_value (language);
}

static const rb_data_type_t rb_eldc_detect_result_type;
/**
 * :call-seq:
 *   detect_details ("TEXT") -> ELDC::DetectResult
 */
static VALUE
rb_eldc_detector_m_detect_details (VALUE self, VALUE text)
{
  const char *text_cstr = rb_string_value_cstr (&text);
  EldcDetectResult *result = malloc (sizeof (EldcDetectResult));
  eldc_detect_details (text_cstr, result);
  return rb_data_typed_object_wrap (rb_cDetectResult, result,
                                    &rb_eldc_detect_result_type);
}

/**
 * :call-seq:
 *   set_languages (nil)
 *   set_languages ([])
 *   set_languages ("")
 *   set_languages ("LANG1,LANG2,...") -> ["LANG1", "LANG2", ...]
 *   set_languages (["LANG1", "LANG2", ...]) -> [...]
 *
 * Empty-ish value resets languages.  If one or more language codes
 * are given, it returns an array of actually applied language code
 * String.  The codes which are not applied is printed to the standard
 * error.
 *
 * Please note that if you write...
 *
 *   codes = (detector.languages = "LANG1,LANG2")
 *
 * ... then the +codes+ is always <tt>"LANG1,LANG2"</tt>.  For
 * checking the returned value, use #set_languages instead.
 */
static VALUE
rb_eldc_detector_m_languages_set (VALUE self, VALUE languages)
{
  (void)self;
  VALUE codes_value;
  switch (rb_type (languages))
    {
    case RUBY_T_NIL:
      codes_value = RUBY_Qnil;
      break;
    case RUBY_T_ARRAY:
      {
        const VALUE *ptr = rb_array_const_ptr (languages);
        const long len = rb_array_len (languages);
        codes_value = rb_str_new (NULL, 0);
        for (long index = 0; index < len; index++)
          {
            if (index)
              rb_str_append (codes_value, rb_str_new_lit (","));
            VALUE language = ptr[index];
            rb_str_append (codes_value, language);
          }
      }
      break;
    default: /* including string */
      codes_value = rb_obj_as_string (languages);
    }
  const char *actual = eldc_set_languages (
      RB_NIL_P (codes_value) ? NULL : rb_string_value_cstr (&codes_value));
  return rb_str_split (rb_str_new_cstr (actual), ",");
}

/**
 * :call-seq:
 *   detector.scheme = "iso639-2t"
 *   detector.scheme = "iso639-1"
 */
static VALUE
rb_eldc_detector_m_scheme_set (VALUE self, VALUE scheme)
{
  (void)self;
  eldc_set_scheme (rb_string_value_cstr (&scheme));
  return RUBY_Qnil;
}

/**
 * :call-seq:
 *   detector.scores = SCORES
 *
 * Controls how many top scores will be returned.  SCORES is clamped
 * between 1 and ELDC::MAX_SCORES.  Default is 3.
 */
static VALUE
rb_eldc_detector_m_scores_set (VALUE self, VALUE scores)
{
  eldc_set_scores (rb_num2int_inline (scores));
  return RUBY_Qnil;
}

/* detect result */

static void
rb_eldc_detect_result_free (void *result)
{
  free (result);
}

static const rb_data_type_t rb_eldc_detect_result_type = {
  .wrap_struct_name = "ELDC detect result",
  .function = { .dfree = rb_eldc_detect_result_free },
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
rb_eldc_detect_result_alloc (VALUE klass)
{
  return rb_data_typed_object_wrap (rb_cDetectResult, NULL,
                                    &rb_eldc_detect_result_type);
}

/**
 * :call-seq:
 *   language => "LANG"
 */
static VALUE
rb_eldc_detect_result_m_language (VALUE self)
{
  EldcDetectResult *result = RTYPEDDATA_DATA (self);
  return rb_str_new_cstr (result->language);
}

/**
 * :call-seq:
 *   reliable -> boolean
 */
static VALUE
rb_eldc_detect_result_m_reliable (VALUE self)
{
  EldcDetectResult *result = RTYPEDDATA_DATA (self);
  return result->reliable ? RUBY_Qtrue : RUBY_Qfalse;
}

static const rb_data_type_t rb_eldc_score_item_type;
/**
 * :call-seq:
 *   scores -> [ELDC::ScoreItem, ...]
 */
static VALUE
rb_eldc_detect_result_m_scores (VALUE self)
{
  EldcDetectResult *result = RTYPEDDATA_DATA (self);
  VALUE scores = rb_ary_new ();
  for (int index = 0; index < result->n_scores; index++)
    {
      VALUE item = rb_data_typed_object_wrap (
          rb_cScoreItem, &result->scores[index], &rb_eldc_score_item_type);
      rb_ary_push (scores, item);
    }
  return scores;
}

/* score item */

static const rb_data_type_t rb_eldc_score_item_type = {
  .wrap_struct_name = "ELDC score item",
  .function = { .dfree = NULL },
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
rb_eldc_score_item_alloc (VALUE klass)
{
  return rb_data_typed_object_wrap (rb_cScoreItem, NULL,
                                    &rb_eldc_score_item_type);
}

/**
 * :call-seq:
 *   language -> "LANG"
 */
static VALUE
rb_eldc_score_item_m_language (VALUE self)
{
  EldcScoreItem *item = RTYPEDDATA_DATA (self);
  return rb_eldc_make_language_value (item->language);
}

/**
 * :call-seq:
 *   score -> Float
 */
static VALUE
rb_eldc_score_item_m_score (VALUE self)
{
  EldcScoreItem *item = RTYPEDDATA_DATA (self);
  return rb_float_new (item->score);
}

RUBY_FUNC_EXPORTED void
Init_eldc (void)
{
  rb_mELDC = rb_define_module ("ELDC");
  rb_define_const (rb_mELDC, "MAX_SCORES",
                   rb_int2num_inline (ELD_LIB_MAX_SCORES));

  rb_cDetector = rb_define_class_under (rb_mELDC, "Detector", rb_cObject);
  rb_define_alloc_func (rb_cDetector, rb_eldc_detector_alloc);
  rb_define_method (rb_cDetector, "initialize", rb_eldc_detector_m_initialize,
                    0);
  rb_define_method (rb_cDetector, "detect", rb_eldc_detector_m_detect, 1);
  rb_define_method (rb_cDetector, "detect_details",
                    rb_eldc_detector_m_detect_details, 1);
  rb_define_method (rb_cDetector, "set_languages",
                    rb_eldc_detector_m_languages_set, 1);
  rb_define_method (rb_cDetector, "scheme=", rb_eldc_detector_m_scheme_set, 1);
  rb_define_method (rb_cDetector, "scores=", rb_eldc_detector_m_scores_set, 1);

  rb_cDetectResult
      = rb_define_class_under (rb_mELDC, "DetectResult", rb_cObject);
  rb_define_alloc_func (rb_cDetectResult, rb_eldc_detect_result_alloc);
  rb_define_method (rb_cDetectResult, "language",
                    rb_eldc_detect_result_m_language, 0);
  rb_define_method (rb_cDetectResult, "reliable",
                    rb_eldc_detect_result_m_reliable, 0);
  rb_define_method (rb_cDetectResult, "scores", rb_eldc_detect_result_m_scores,
                    0);

  rb_cScoreItem = rb_define_class_under (rb_mELDC, "ScoreItem", rb_cObject);
  rb_define_alloc_func (rb_cScoreItem, rb_eldc_score_item_alloc);
  rb_define_method (rb_cScoreItem, "language", rb_eldc_score_item_m_language,
                    0);
  rb_define_method (rb_cScoreItem, "score", rb_eldc_score_item_m_score, 0);
}
