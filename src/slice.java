public class Slice<T>{
    public int cap;
    public int len;
    
    private T[] elementList;

    public Slice(){
        elementList = new T[0];
        cap = 0;
        len = 0;
    }

    public void append(T element){
        if(len == cap){
            doubleSize();
        }
        t[len] = element;
        len++;
    }

    public void doubleSize(){
        T[] newElementList;
        int oldCap = cap;
        if(cap == 0){
            newElementList = new T[2]; 
            cap = 2;
        } else {
            newElementList = new T[cap * 2];
            cap *= 2;
        }
        // Copy over old elemnets
        for(int i = 0; i < oldCap; i++){
            newElementList[i] = elementList[i];
        }
        elementList = newElementList;
    }
    
    public T get(int index){
        if(index < 0){
        
        } else if(index > cap){

        } else if (index > len){
            
        } else {
            return elementList[index];
        }
    }
}