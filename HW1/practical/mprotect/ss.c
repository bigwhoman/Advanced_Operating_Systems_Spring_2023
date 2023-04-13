void* Func (void *args){
    void *Arr[1000000];
    for (int a =0;a < 1000000;a++){
        for (int b = 0; b < 1000000; b++)
        {
            Arr[b] = malloc(1048576);
            for (int c = 0; c < 1000; c++)
            {
                Free(Arr[999999-c]);
            }
            
        }
        
    }
}