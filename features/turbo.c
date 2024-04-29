#include QMK_KEYBOARD_H
#include "print.h"
#include "features/turbo.h"

#if !defined(DEFERRED_EXEC_ENABLE)
#error "turbo: Please set `DEFERRED_EXEC_ENABLE = yes` in rules.mk."
#else

//static uint16_t turbo_keycodes[] = {KC_Q, KC_W};
static uint16_t turbo_keycodes_q = KC_Q;
static uint16_t turbo_keycodes_w = KC_W;
static deferred_token q_token = INVALID_DEFERRED_TOKEN;
static deferred_token w_token = INVALID_DEFERRED_TOKEN;
static bool q_registered = false;
static bool w_registered = false;

static uint32_t turbo_callback(uint32_t trigger_time, void* cb_arg) {
  bool* registered;
  uint16_t keycode = *((uint16_t*)cb_arg);

  switch (keycode) {
    case KC_Q:
      registered = &q_registered;
      break;
    case KC_W:
      registered = &w_registered;
      break;
    default:
      registered = NULL;
      break;
  }

  if (*registered) {
    unregister_code16(keycode);
    *registered = false;
  } else {
    *registered = true;
    register_code16(keycode);
  }

  return 50 + rand() % 11;
}

static void turbo_start(uint16_t keycode) {
  switch(keycode) {
    case KC_Q:
      if (q_token == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&turbo_keycodes_q);
        q_token = defer_exec(next_delay_ms, turbo_callback, (void*)&turbo_keycodes_q);
      }
      break;
    case KC_W:
      if (w_token == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&turbo_keycodes_w);
        w_token = defer_exec(next_delay_ms, turbo_callback, (void*)&turbo_keycodes_w);
      }
      break;
  }
}

static void turbo_stop(uint16_t keycode) {
  switch (keycode) {
    case KC_Q:
      if (q_token != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(q_token);
        q_token = INVALID_DEFERRED_TOKEN;
        if (q_registered) {
          unregister_code16(KC_Q);
          q_registered = false;
        }
      }
      break;
    case KC_W:
      if (w_token != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(w_token);
        w_token = INVALID_DEFERRED_TOKEN;
        if (w_registered) {
          unregister_code16(KC_W);
          w_registered = false;
        }
      }
    break;
  }
}

bool process_turbo(uint16_t keycode, keyrecord_t* record) {
  if (record->event.pressed) {
    turbo_start(keycode);
  } else {
    turbo_stop(keycode);
  }
  return true;
}

#endif
