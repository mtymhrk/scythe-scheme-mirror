#ifndef INCLUDE_REPL_CODE_H__
#define INCLUDE_REPL_CODE_H__

static const char *repl_code = "((undef) (asm-close 0 0 ((cframe lbl_call-cont_1) (asm-close 0 1 ((asm-close 1 1 ((cframe lbl_call-cont_2) (sref 0 0) (push) (gref display (scythe internal repl)) (call 1) (nop) (label lbl_call-cont_2) (cframe lbl_call-cont_3) (gref newline (scythe internal repl)) (call 0) (nop) (label lbl_call-cont_3) (immval #t) (push) (sref 0 1) (tcall 1))) (push) (asm-close 1 0 ((cframe lbl_call-cont_4) (immval \"> \") (push) (gref display (scythe internal repl)) (call 1) (nop) (label lbl_call-cont_4) (cframe lbl_call-cont_5) (gref flush-output-port (scythe internal repl)) (call 0) (nop) (label lbl_call-cont_5) (cframe lbl_call-cont_6) (gref read (scythe internal repl)) (call 0) (mrve) (label lbl_call-cont_6) (push) (eframe 1) (emine 1) (asm-close 3 1 ((cframe lbl_call-cont_9) (sref 0 0) (push) (gref eof-object? (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_9) (jmpf lbl_if-a_8) (cframe lbl_call-cont_10) (immval #f) (push) (sref 0 3) (call 1) (nop) (label lbl_call-cont_10) (jmp lbl_if-j_7) (label lbl_if-a_8) (undef) (label lbl_if-j_7) (cframe lbl_call-cont_11) (sref 0 0) (push) (gref eval (scythe internal repl)) (call 1) (nop) (label lbl_call-cont_11) (mrvc -1) (mvpush) (eframe 1) (cframe lbl_call-cont_14) (sref 0 0) (push) (gref null? (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_14) (jmpf lbl_if-a_13) (cframe lbl_call-cont_15) (gref newline (scythe internal repl)) (call 0) (nop) (label lbl_call-cont_15) (jmp lbl_if-j_12) (label lbl_if-a_13) (sref 0 0) (push) (eframe 1) (emine 1) (asm-close 1 1 ((cframe lbl_call-cont_17) (sref 0 0) (push) (gref null? (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_17) (jmpf lbl_if-a_16) (undef) (return) (label lbl_if-a_16) (cframe lbl_call-cont_18) (cframe lbl_call-cont_19) (sref 0 0) (push) (gref car (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_19) (push) (gref write (scythe internal repl)) (call 1) (nop) (label lbl_call-cont_18) (cframe lbl_call-cont_20) (gref newline (scythe internal repl)) (call 0) (nop) (label lbl_call-cont_20) (cframe lbl_call-cont_21) (sref 0 0) (push) (gref cdr (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_21) (push) (sref 0 1) (tcall 1))) (demine 0 0) (cframe lbl_call-cont_22) (sref 0 1) (push) (sref 0 0) (call 1) (nop) (label lbl_call-cont_22) (epop) (epop) (label lbl_if-j_12) (epop) (cframe lbl_call-cont_23) (immval \"> \") (push) (gref display (scythe internal repl)) (call 1) (nop) (label lbl_call-cont_23) (cframe lbl_call-cont_24) (gref flush-output-port (scythe internal repl)) (call 0) (nop) (label lbl_call-cont_24) (cframe lbl_call-cont_25) (gref read (scythe internal repl)) (call 0) (mrve) (label lbl_call-cont_25) (push) (sref 0 1) (tcall 1))) (demine 0 0) (sref 0 1) (push) (sref 0 0) (tcall 1))) (push) (gref with-exception-handler (scythe internal repl)) (tcall 2))) (push) (gref call/cc (scythe internal repl)) (call 1) (mrve) (label lbl_call-cont_1) (jmpf lbl_if-a_0) (gref read-eval-print-loop (scythe internal repl)) (tcall 0) (label lbl_if-a_0) (undef) (return))) (gdef read-eval-print-loop (scythe internal repl)))";

#endif  /*  INCLUDE_REPL_CODE_H__ */
