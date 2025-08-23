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
Each warp has a further 128 bytes of shared registers, mapped starting at address 128. Warp controller unit also has access to 128 bytes of individual register memory

Accumulator is a separate register

## core types

- Manager Core:
  - Holds the instruction pointer and manages the lowest level cache
  - Broadcasts instructions to the executor cores
  - Has its own instructions for executor core management and some very basic integer arithmetic
  - Cannot access memory directly except for the 128 bytes of shared warp registers
  - Can enable and disable any cores
- Executor Cores:
  - Can execute 32 bit floating point and integer instructions
  - Can disable itself
  - There are 16 of them under a manager core

## instructions

|name | opcode | description |
|---|---|---|
| category | broadcastable instructions |
| no operation | NOP | do nothing |
| move | MOV \<R: m\> \<A: l/m\> | Move literal A into location R: R = A |
| accumulator flush | ACF \<type\> \<R: m\> | flush accumulator to the desired memory location, set accumulator to 0: R = ACC; ACC = 0 |
| multiply-add-accumulate | MAC \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> \<C: l/m\>| fused multiply add: ACC += A*B+C |
| fused multiply-add | FMA \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> \<C: l/m\>| fused multiply add: R = A*B+C |
| negate | NEG \<type\> \<R: m\> \<A: l/m\> | Negate the value of A: R = A * -1 |
| divide | DIV \<type\> \<A: m\> \<B: m\> | Divide A by B; store result in A, store remainder in B: A,B = A/B,A%B |
| convert | CVT \<R type\> \<A type\> \<R: m\> \<A: l/m\> | Convert A from a type to another and store the result in R. Can set overflow flag as needed |
| compare | CMP \<type\> \<A: m\> \<B: m\> | Compares A and B and sets the FLAGS register appropriately |
| conditional core disable | CDS \<C: m\> | disable core if C \| FLAGS == 0xFFFF |
|||# TODO - NOT FINISHED, ADD MORE ARITHMETIC INSTRUCTIONS |
| category | controller core instructions | Note: |
| core enable | NBC_CEN \<M: l\> | Enable cores based on mask M |
| core invert | NBC_CIV | Disable enabled cores and enable disabled cores |
| unconditional jump | NBC_JMP \<L: l/m\>| Jump to location L | 
| conditional jump | NBC_CJM \<M: l/m\> \<L: l/m\>| Jump to location L if CORE_MASK == M |
| bitwise OR  | NBC_OR  \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> | Bitwise OR:  R = A \| B |
| bitwise AND | NBC_AND \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> | Bitwise AND: R = A  & B |
| bitwise NOT | NBC_NOT \<type\> \<R: m\> \<A: l/m\>            | Bitwise NOT: R = ~A     |
| multiply-add-accumulate | NBC_MAC \<type\> \<R: m\> \<A: l/m\> \<B: l/m\> \<C: l/m\>| fused multiply add: ACC += A*B+C |
