int recursive(int i){
    int x = i;
	
    if(x > 0){
        x--;
        x = recursive(x);
    }
	
    return x;
}