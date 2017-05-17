//
// Created by zhs007 on 2017/5/16.
//

#ifndef HOLDEM_CORE_CMD_H
#define HOLDEM_CORE_CMD_H

#include <string>
#include <vector>
#include <cstring>
#include <functional>
#include <map>

struct Command{
    std::string                 strCmd;
    std::vector<std::string>    lstParam;
};

typedef std::function< void(Command&) > FuncCommand;

class CommandMgr{
    typedef std::map<std::string, FuncCommand> _Map;
    typedef std::pair<std::string, FuncCommand> _Pair;
public:
    static CommandMgr& getSingleton();
public:
    Command* newCommand(const char* cmd);

    void deleteCommand(Command* pCmd);

    void procCommand(Command& cmd);

    void regCommand(const char* str, FuncCommand func);

    void getCmd(char* strbuf, int len);
protected:
    CommandMgr();
    ~CommandMgr();
protected:
    _Map m_map;
};

#endif //HOLDEM_CORE_CMD_H
