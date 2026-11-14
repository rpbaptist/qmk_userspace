# Flow Tap on the shift thumb keys

The keymaps do not set `FLOW_TAP_TERM`. `SFT_SPC` and `SFT_ENT` keep the
current tap-hold settings: `TAPPING_TERM 140` (`SFT_ENT` 120), and
`PERMISSIVE_HOLD`.

## Why this is out of scope

Flow Tap fixes a problem that we have not seen. It is a speculative change:
there is no record of SHIFT when we want SPACE or ENTER during fast typing.

It also has a cost. With Flow Tap, a hold of `SFT_SPC` within
`FLOW_TAP_TERM` after a letter becomes SPACE, not SHIFT. This breaks fast
mid-word capitals and camelCase on right-hand letters.

The plain define also does not do what the request expects. The default
`get_flow_tap_term` applies only when the current key and the previous key
are both "flow tap keys" (`quantum/action_tapping.c`):

```c
switch (get_tap_keycode(keycode)) {
    case KC_SPC:
    case KC_A ... KC_Z:
    case KC_DOT:
    case KC_COMM:
    case KC_SCLN:
    case KC_SLSH:
        return true;
}
```

`SFT_ENT` taps `KC_ENT`, so it needs a custom `get_flow_tap_term` too.

## When to reconsider

Reconsider if SPACE or ENTER misfires as SHIFT during real typing. Then
decide if `SFT_ENT` is in scope (custom `get_flow_tap_term`), and if the
Planck keymap, which has the same keys, gets the same change. Add a host
test for "letter, then `SFT_SPC` hold within `FLOW_TAP_TERM`".

## Prior requests

- #11: "crkbd: Consider FLOW_TAP_TERM for shift thumb keys"
