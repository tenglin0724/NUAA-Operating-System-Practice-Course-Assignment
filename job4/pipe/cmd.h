#ifndef _CMD_H
#define _CMD_H
#define MAX_ARGC 10

struct cmd{
	int argc;
	char*input;
	char *output;
	char *argv[MAX_ARGC];
};
extern void exec_pipe_cmd(int cmdc,int start, struct cmd *cmdv);
extern void exec_cmd(struct cmd *cmd);
extern int builtin_cmd(struct cmd*cmd);
#endif
