# GPU ISA spec

## operand type legend
| letter | definition |
|---|---|
| r | register |
| m | memory   |
| l | literal  |

### command types
| code | definition |
|---|---|
| F32 | 32 bit float |
| F16 | 16 bit float |
| I32 | 32 bit integer |
| I16 | 16 bit integer |
| I8  |  8 bit integer |

Float and Integer systems have separate pipes, but registers are shared. Using types smaller than 32 bit will implicitly convert them to 32 bits and will use their pipes

## registers and non-uniform memory

Each executor has 16 general purpose 32 bit registers, named r0 through r15. Operations on types smaller than 32 bits will use the lower (least significant) part of the register (unless using packed instructions - will see if those will be added).

Accumulator is a separate register which can only be accessed using the ACF instruction.

FLAGS register structure:
| bit idx | value |
|---|---|
|  0 | Carry / Overflow |
|  1 | Result Zero  |
|  2 | Sign Change  |
|  2 | Sign Change  |
|  3 | Core Enabled |
|  4-31 | Reserved  |

Each core has 128 bytes of high speed local memory, mapped starting at address 0 and can be accessed like regular memory.
Each warp has a 512 bytes of shared local memory, of which 384 are accessible to the executor cores starting at address 128. The first 128 bytes are exclusive to the warp controller


## Architecture layers

- Management and Dispatch engine
  - Interfaces with the OS and manages running threads and jobs
  - Issues context switches to the multiprocessors
- Multiprocessor / Warp:
  - Contains one control unit which holds the instruction pointer and decodes the instructions
  - Broadcasts decoded instructions to the executor cores
  - Has its own instructions for executor core management and some very basic integer arithmetic
  - Cannot access memory directly except for the 512 bytes of shared warp memory
- Executor Cores:
  - Consumes microcode
  - Can execute 32 bit floating point and integer instructions
  - There are 16 of them under a manager core

## instructions

|name | opcode | description |
|---|---|---|
| category | broadcastable instructions |
| no operation | NOP | do nothing |
| move | MOV \<R: r/m\> \<A: r/m/l\> | Move literal A into location R: R = A |
| accumulator flush | ACF \<type\> \<R: r/m\> | flush accumulator to the desired memory location, set accumulator to 0: R = ACC; ACC = 0 |
| multiply-add-accumulate | MAC \<type\> \<R: r/m\> \<A: r/m/l\> \<B: r/m/l\> \<C: r/m/l\>| fused multiply add: ACC += A*B+C |
| fused multiply-add | FMA \<type\> \<R: r/m\> \<A: r/m/l\> \<B: r/m/l\> \<C: r/m/l\>| fused multiply add: R = A*B+C |
| negate | NEG \<type\> \<R: r/m\> \<A: r/m/l\> | Negate the value of A: R = A * -1 |
| divide | DIV \<type\> \<A: r/m\> \<B: r/m\> | Divide A by B; store result in A, store remainder in B: A,B = A/B,A%B |
| convert | CVT \<R type\> \<A type\> \<R: r/m\> \<A: r/m\> | Convert A from a type to another and store the result in R. Can set overflow flag as needed |
| compare | CMP \<type\> \<A: r/m/l\> \<B: r/m/l\> | Compares A and B and sets the FLAGS register appropriately |
| rotate | ROT \<R: r\> \<A: r/m/l\> | Rotate register R by A |
| bitwise and | AND \<R: r/m\> \<A: r/m/l\> \<B: r/m/l\> | R = A  & B |
| bitwise or  | OR  \<R: r/m\> \<A: r/m/l\> \<B: r/m/l\> | R = A \| B |
| bitwise not | NOT \<R: r/m\> \<A: r/m/l\> | R = ~A |
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
