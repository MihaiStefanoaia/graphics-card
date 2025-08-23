# GPU ISA spec

## operand type legend
| letter | definition |
|---|---|
| l | literal|
| m | memory |

### command types
| code | definition |
|---|---|
| F32 | 32 bit float |
| F16 | 16 bit float |
| I32 | 32 bit integer |
| I16 | 16 bit integer |
| I8  |  8 bit integer |

Float and Integer systems have separate pipes, but registers are shared. Using types smaller than 32 bit will implicitly convert them to 32 bits and will use their pipes

## registers

Each core has 128 bytes of general purpose register memory, memory mapped starting at address 0 and can be accessed like regular memory.


Accumulator is a separate register

## instructions

|name | opcode | description |
|---|---|---|
| no operation | NOP | do nothing |
| move | MOV \<R: m\> \<A: l/m\> | Move literal A into location R: R = A |
| accumulator flush | ACF \<type\> \<R: m\> | flush accumulator to the desired memory location, set accumulator to 0: R = ACC; ACC = 0 |
| multiply-add-accumulate | MAC \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> \<C: l/m\>| fused multiply add: ACC += A*B+C |
| fused multiply-add | FMA \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> \<C: l/m\>| fused multiply add: R = A*B+C |
| negate | NEG \<type\> \<R: m\> \<A: l/m\> | Negate the value of A: R = A * -1 |
| divide | DIV \<type\> \<A: m\> \<B: m\> | Divide A by B; store result in A, store remainder in B: A,B = A/B,A%B |
| convert | CVT \<R type\> \<A type\> \<R: m\> \<A: l/m\> | Convert A from a type to another and store the result in R. Can set overflow flag as needed |

# TODO - NOT FINISHED, ADD MORE INSTRUCTIONS AND FIGURE OUT WARPS
