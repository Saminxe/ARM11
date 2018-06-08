start:
  ldr r0,=0x20200004 ; physical address of pins 10-19
  mov r1,#4 ; move 0b0100 into r1.
  str r1,[r0,#2] ; stores 0b0100 into 0x020200007, sets as output
  ldr r2,=0x20200028 ; clear register
  ldr r3,=0x2020001C ; set register
  mov r4,#0xFF ; loop counter
  mov r5,#1 ; move 0b0001 into r5
  mov r6,#0
loop:
  str r6,[r4,#2]
  str r5,[r3,#2]
  str r5,[r4,#2]
  str r6,[r3,#2]
  sub r4,r4,#1
  cmp r4,#0
  bne loop
end:
  andeq r0,r0,r0
