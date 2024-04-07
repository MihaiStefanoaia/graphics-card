# Custom GPU spec - CUDAish in nature

## Basic high level info

4 tiers of execution:
- Governor core: main dispatch
  - manages tasks (subroutines)
  - dispatches the tasks to the accessor core
  - manages memory - contains the main mmu
- Accessor core: memory interface
  - manages 4 decoder core "threads"
  - manages shared l2 cache
  - has a 16 byte data bus to memory
- Decoder core: smart executor
  - decodes instructions into microcode
  - hosts the shared instruction pointer
  - contains its task queue
  - manages L1i and L1d cache
  - may be able to feed ooo / speculative execution to executor cores
  - manages 8 executor cores
- Executor core: dumb executor
  - fed microcode by the decoder core
  - executes whatever instruction it receives
  - basic pipelining, but no ooo or speculative execution (on its own)
  - memory mapped registers


Task structure:
- Contains the following data:
  - task id
  - argc
  - ptr argv  (argv[0] always points to the instructions)
  - bufsize argv
- Is dispatched as an interrupt message with a pointer to the above data
- No memory allocation is allowed during its execution
