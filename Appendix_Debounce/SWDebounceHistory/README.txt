SWDebounce

This sketch uses no extra hardware to facilitate switch  debouncing as explained in the Appendix. The debounce is purely software and rather than wait for a specific period -- which may be too short/too long for some other switches -- uses a repeated lop reading the switch until it reads 32 consecutive HIGHs or 32 consecutive LOWs to indicate that the switch has stopped bouncing.


