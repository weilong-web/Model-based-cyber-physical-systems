# Model-based-cyber-physical-systems
Modelling and Simulation

Explain the difference between casual and acasual modelling of dynamic systems. (P2)

Demonstrate knowledge on how to build models of hybrid systems using the Modelica/Simscape language. The focus is not on the syntax of Modelica/Simscape language but on the general principles used to build models in Modelica/Simscape. (P2)

Describe how hybrid systems can be simulated using Simulink. (IA1)

Modelling of hybrid systems using hybrid automata. (IA1)

Describe how Zeno-behavior might occur and how it might affect the simulation of hybrid systems. (IA1)

Describe why zero-crossing detection is important when simulating hybrid systems. (IA1)

Microcontrollers, real-time operating systems, and scheduling

Explain the difference between storing the value of a variable and storing the memory address to a variable. (IA2, P4)

Describe how to access the address of a variable and how to access the value of the variable if a pointer to the variable is given in C. (IA2, P4)

Explain the difference between pass by value (send the actual value as the argument) and pass by reference (sending a pointer, i.e. the address) for interaction with sub-routines. (IA2, P4)

Describe situations when multiple tasks are useful. (IA2, P4, P5)

Describe the differences between different forms of tasks, i.e., threads (shared memory) and processes (no shared memory). (IA2, P4)

Describe the general concepts of a real-time operating system, i.e. scheduler, tasks, priorities, context switches.  (IA2, P4)

Being able to write and interpret simple embedded programs using threads, semaphore and pointers implemented in C. (IA2, P4, P5)

Describe why mutual exclusion between multiple threads is necessary to avoid race conditions and how it can be achieved using mutual exclusion locks. (IA2, P4)

Explain how mutual exclusion might lead to deadlock situations. (IA2, P4)

Describe the four necessary conditions for deadlocks to occur in multi-tasks programs. 

Explain how to structure access to mutual exclusion locks in multi-threaded programs such that deadlocks might be avoided.

Explain how semaphores are defined and how they can be used to implement mutual exclusion and synchronization among tasks.

Describe how task and synchronization of tasks can be implemented using FreeRTOS.

Describe the differences between off-line (cyclic executives) and online schedulers (priority or deadline-based) and situations where it might be appropriate to use them.

Define what characterizes a preemptive scheduler.

Describe how the priority of a task influences the decisions taken by the scheduler.

Define times (release time, response time, finish time, and deadline) associated with the execution of a task.

Ability to apply and analyze the behavior of schedules set by the rate and deadline monotonic principle. Explain under which conditions is the schedule optimal. Apply the exact condition for schedulability and calculation of response times. (IA3)

Explain under what conditions priority inversion might happen. Explain how the priority inheritance protocol can solve the priority inversion problem.

Ability to apply and analyze the earliest deadline first principle.

Implementation of controllers

Describe how the scheduler affects the latency and jitter of concurrent control tasks. Being to divide control tasks into sub-tasks that are time-critical and non-time-critical tasks and that are scheduled separately. (P5)

Describe how latency can affect the design of control algorithms. (P5)
Being able to implement filters and state-feedback controllers in C. (P1, P5)
Modeling of specifications and requirements
Syntax and semantics of linear temporal logic expressions (LTL-expressions). (IA3)
Being able to formulate and interpret safety and liveness specifications in linear temporal logic. (IA3)
