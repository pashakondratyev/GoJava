class Helper {
    public static boolean arrayComparison(Integer[] a, Integer[] b){
        for(int i = 0; i < a.length; i++){
            if(!a[i].equals(b[i])){
                return false;
            }
        }
        return true; 
    }

    @Override
    public boolean equals(Object a){
        
    }
}