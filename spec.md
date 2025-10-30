# Custom GPU spec - CUDAish in nature

## Basic high level info

3 tiers of execution:
- Management engine
  - manages tasks dispatched by the 
  - dispatches the tasks to the accessor core
- Decoder core
  - decodes instructions into microcode
  - hosts the shared instruction pointer
  - manages 16 executor cores
- Executor core: dumb executor
  - fed microcode by the decoder core
  - executes whatever instruction it receives
  - basic pipelining, maybe ooo execution but no speculative execution
  - memory mapped registers


Task structure:
- Contains the following data:
  - task id
  - argc
  - ptr argv  (argv[0] always points to the instructions)
  - bufsize argv
- Is dispatched as an interrupt message with a pointer to the above data
- No memory allocation is allowed during its execution
