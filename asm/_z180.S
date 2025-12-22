.code

.export _out16
.export _in16

.z180
_out16:
  push  bc
  ld    hl,#4
  add   hl,sp
  ld    c,(hl)
  inc   hl
  ld    b,(hl)
  inc   hl
  ld    e,(hl)
  out   (c),e
  pop   bc
  ret

_in16:
  push  bc
  ld    hl,#4
  add   hl,sp
  ld    c,(hl)
  inc   hl
  ld    b,(hl)
  in    l, (c)
  ld    h,0
  pop   bc
  ret
