#include QMK_KEYBOARD_H
#include "print.h"
#include "features/turbo.h"

#if !defined(DEFERRED_EXEC_ENABLE)
#error "turbo: Please set `DEFERRED_EXEC_ENABLE = yes` in rules.mk."
#else


static uint16_t keycodes[] = {KC_Q, KC_W, KC_R, KC_T};
static deferred_token tokens[] = {INVALID_DEFERRED_TOKEN, INVALID_DEFERRED_TOKEN, INVALID_DEFERRED_TOKEN, INVALID_DEFERRED_TOKEN};
static bool registereds[] = {false, false, false, false};

static uint32_t turbo_callback(uint32_t trigger_time, void* cb_arg) {
  bool registered;
  uint16_t keycode = *((uint16_t*)cb_arg);

  switch (keycode) {
    case KC_Q:
      registered = registereds[0];
      break;
    case KC_W:
      registered = registereds[1];
      break;
    case KC_R:
      registered = registereds[2];
      break;
    case KC_T:
      registered = registereds[3];
      break;
    default:
      registered = NULL;
  }

  if (registered) {
    unregister_code16(keycode);
    registered = false;
  } else {
    registered = true;
    register_code16(keycode);
  }

  switch (keycode) {
    case KC_Q:
      registereds[0] = registered;
      break;
    case KC_W:
      registereds[1] = registered;
      break;
    case KC_R:
      registereds[2] = registered;
      break;
    case KC_T:
      registereds[3] = registered;
      break;
  }

  return 50 + rand() % 11;
}

static void turbo_start(uint16_t keycode) {
  switch(keycode) {
    case KC_Q:
      if (tokens[0] == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&keycodes[0]);
        tokens[0] = defer_exec(next_delay_ms, turbo_callback, (void*)&keycodes[0]);
      }
      break;
    case KC_W:
      if (tokens[1] == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&keycodes[1]);
        tokens[1] = defer_exec(next_delay_ms, turbo_callback, (void*)&keycodes[1]);
      }
      break;
    case KC_R:
      if (tokens[2] == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&keycodes[2]);
        tokens[2] = defer_exec(next_delay_ms, turbo_callback, (void*)&keycodes[2]);
      }
      break;
    case KC_T:
      if (tokens[3] == INVALID_DEFERRED_TOKEN) {
        uint32_t next_delay_ms = turbo_callback(0, (void*)&keycodes[3]);
        tokens[3] = defer_exec(next_delay_ms, turbo_callback, (void*)&keycodes[3]);
      }
      break;
  }
}

static void turbo_stop(uint16_t keycode) {
  switch (keycode) {
    case KC_Q:
      if (tokens[0] != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(tokens[0]);
        tokens[0] = INVALID_DEFERRED_TOKEN;
        if (registereds[0]) {
          unregister_code16(KC_Q);
          registereds[0] = false;
        }
      }
      break;
    case KC_W:
      if (tokens[1] != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(tokens[1]);
        tokens[1] = INVALID_DEFERRED_TOKEN;
        if (registereds[1]) {
          unregister_code16(KC_W);
          registereds[1] = false;
        }
      }
      break;
    case KC_R:
      if (tokens[2] != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(tokens[2]);
        tokens[2] = INVALID_DEFERRED_TOKEN;
        if (registereds[2]) {
          unregister_code16(KC_R);
          registereds[2] = false;
        }
      }
      break;
    case KC_T:
      if (tokens[3] != INVALID_DEFERRED_TOKEN) {
        cancel_deferred_exec(tokens[3]);
        tokens[3] = INVALID_DEFERRED_TOKEN;
        if (registereds[3]) {
          unregister_code16(KC_T);
          registereds[3] = false;
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
