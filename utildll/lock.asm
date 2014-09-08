        .386p
_TEXT         segment para public use32 'CODE'
           assume cs:_TEXT
           assume ds:nothing

        public lockincr
            public lockdecr
            public lockxchng
        

 lockincr proc    near
         mov     eax,[esp+4]
           lock inc dword ptr [eax]
           ret
 lockincr endp
 
 lockdecr proc    near
           mov eax,[esp+4]
           lock dec dword ptr [eax]
         ret
 lockdecr endp
 
 lockxchng  proc    near
           mov ecx,[esp+4]
           mov eax,[esp+8]
           lock xchg eax,[ecx]
           ret
 lockxchng  endp
 
 _TEXT             ends

         end
 
