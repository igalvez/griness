.include "instr_test.inc"

instrs:
	entry $E0,"CPX #n"
instrs_size = * - instrs

operand = instr+1

instr_template:
	lda #0
	jmp instr_done
instr_template_size = * - instr_template

.define set_in    set_paxyso
.define check_out check_paxyso

.include "instr_test_end.s"

test_values:
	test_normal
	rts

correct_checksums:
.dword $5D5728B8
.dword $EA228F76
.dword $7C0C60CB
.dword $49288BFC
.dword $14C7EA46
.dword $42684E66
.dword $EA1D7F06
.dword $512F9D2A
.dword $70AA1B34
.dword $D3DC4002
.dword $6675067C
.dword $14C7EA46
.dword $6CB13BC0
.dword $6CB13BC0
.dword $6CB13BC0
.dword $6CB13BC0
.dword $6CB13BC0
.dword $FE191060
.dword $FE191060
.dword $27355577
.dword $C6B8642B
.dword $D311C870
.dword $EE21BFAD
