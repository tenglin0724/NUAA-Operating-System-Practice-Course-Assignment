#ifndef _PARSE_H
#define _PARSE_H

#include "cmd.h"

extern void parse_cmd(char *line,struct cmd *cmd);
extern void dump_cmp(struct cmd*cmd);

#endif
