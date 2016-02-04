#!/usr/sbin/dtrace -s

#pragma D option quiet

BEGIN
{
  printf("Start dtrace script: time %ul\n\n", walltimestamp);
}

proc:::exec-success
/ execname == "QFreeRTOS" /        
{
  trace(execname);
}


sched:::off-cpu
/ execname == "QFreeRTOS" /        
{
  printf("tid: %ul (%s), off-cpu at: %ul\n", tid, curthread->td_name, walltimestamp); 
}

sched:::on-cpu
/ execname == "QFreeRTOS" /        
{
  printf("tid: %ul, on-cpu at: %ul\n", tid, walltimestamp); 
}