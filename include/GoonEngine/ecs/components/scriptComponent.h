#pragma once

typedef void (*ScriptComponentDelegate)(void* data);

typedef struct ScriptComponent
{
    ScriptComponentDelegate UpdateFunc;
} ScriptComponent;

ScriptComponent* gnScriptComponentNew();
int gnScriptComponentSetUpdateFunc(ScriptComponent* scriptComponent, ScriptComponentDelegate func);