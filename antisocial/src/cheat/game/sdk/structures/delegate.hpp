#pragma once

class c_delegate
{
private:
    char pad[ 16 ];

public:
    void* method_ptr;
    void* invoke_impl;
    void* m_target;
    void* method;
    void* delegate_trampoline;
    void* method_code;
    void* method_info;
    void* original_method_info;
    void* data;

    void hook( void* hook_func, void** orig_func )
    {
        if ( *orig_func == nullptr )
        {
            *orig_func = this->invoke_impl;
        }

        this->invoke_impl = hook_func;
    }
};