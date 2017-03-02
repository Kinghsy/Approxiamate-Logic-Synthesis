//
// Created by tripack on 17-3-1.
//

#ifndef VE490_ABC_H
#define VE490_ABC_H

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
extern void   Abc_Start();
extern void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
extern void * Abc_FrameGetGlobalFrame();
extern int    Cmd_CommandExecute( void * pAbc, char * sCommand );

#endif //VE490_ABC_H
