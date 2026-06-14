#ifndef LV_I18N_H
#define LV_I18N_H

#ifdef __cplusplus
extern "C" {
#endif

#define _(x) (x)

static inline void lv_i18n_init(void) {}
static inline void lv_i18n_set_locale(const char * l) { (void)l; }

#ifdef __cplusplus
}
#endif

#endif