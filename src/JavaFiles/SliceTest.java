public class SliceTest{
    static public void testCreate(){
        Slice<Integer> intSlice = new Slice<>();
        assert(intSlice.cap == 0);
        assert(intSlice.len == 0);
        assert(intSlice.elementList.size() == 0);
    }
    
    static public void testAppend(){
        Slice<Integer> intSlice = new Slice<>();
        Slice<Integer> intSlice2 = intSlice.append(1);

        assert(intSlice != intSlice2);
        assert(intSlice.cap == 0);
        assert(intSlice.len == 0);

        assert(intSlice2.cap == 2);
        assert(intSlice2.len == 1);
    }

    static public void testAppend2(){
        Slice<Integer> intSlice = new Slice<>();
        intSlice = intSlice.append(1);
        assert(intSlice.cap == 2);
        assert(intSlice.len == 1);
        intSlice = intSlice.append(2);
        assert(intSlice.cap == 2);
        assert(intSlice.len == 2);
        intSlice = intSlice.append(3);
        assert(intSlice.cap == 4);
        assert(intSlice.len == 3);
    }

    static public void testAppend3(){
        Slice<Integer> intSlice = new Slice<>();
        intSlice = intSlice.append(1);
        Slice<Integer> intSlice2 = intSlice.copy();
        
        intSlice = intSlice.append(2);
        assert(intSlice.elementList.get(1) == 2);

        intSlice2 = intSlice2.append(3);
        assert(intSlice.elementList.get(1) == 3);
        assert(intSlice2.elementList.get(1) == 3);
    }

    static public void testAppend4(){
        Slice<Integer> intSlice = new Slice<>();
        intSlice = intSlice.append(1);
        Slice<Integer> intSlice2 = intSlice.append(2);
        assert(intSlice != intSlice2);
        assert(intSlice.len != intSlice2.len);
    }

    public static void main(String[] args) {
        testCreate();
        testAppend();
        testAppend2();
        testAppend3();
        testAppend4();
    }
}