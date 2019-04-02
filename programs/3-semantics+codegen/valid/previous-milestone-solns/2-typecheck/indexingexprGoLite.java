import java.util.Arrays;

import java.util.ArrayList;
import java.lang.ArrayIndexOutOfBoundsException;

class Slice<T>{
    public int cap;
    public int len;

    public ArrayList<T> elementList;

    public Slice() {
        elementList = new ArrayList<>(0);
        cap = 0;
        len = 0;
    }

    public Slice<T> append(T element) {
        if (len == cap) {
            ArrayList<T> newElementList;
            int newCap = 0;
            if (cap == 0) {
                newElementList = new ArrayList<>(2);
                newCap = 2;
            } else {
                newElementList = new ArrayList<>(cap * 2);
                newCap = cap * 2;
            }
            // Copy over old elements
            for (int i = 0; i < cap; i++) {
                newElementList.add(i, elementList.get(i));
            }
            Slice<T> newSlice = new Slice<>();
            newSlice.cap = newCap;
            
            newElementList.add(len, element);
            newSlice.len = len + 1;
            newSlice.elementList = newElementList;
            return newSlice;
        } else{
            elementList.add(len, element);
            Slice<T> newSlice = new Slice<>();
            newSlice.len = len + 1;
            newSlice.cap = cap;
            newSlice.elementList = elementList;
            return newSlice;
        }
    }

    public Slice<T> copy() {
        Slice<T> copy = new Slice<>();
        copy.cap = this.cap;
        copy.len = this.len;
        copy.elementList = this.elementList;
        return copy;
    }

    // TODO: handle the different cases with appropriate errors
    public T get(int index) throws ArrayIndexOutOfBoundsException {
        if (index < 0) {
            throw new ArrayIndexOutOfBoundsException("Slice index must be non-negative.");
        } else if (index >= cap) {
            throw new ArrayIndexOutOfBoundsException("Index must be less than the capacity of the slice.");
        } else if (index >= len) {
            throw new ArrayIndexOutOfBoundsException("Index must be less than the length of the slice.");
        } else {
            return elementList.get(index);
        }
    }

    // TODO: handle the different cases with appropriate errors
    public void put (int index, T element) throws ArrayIndexOutOfBoundsException {
        if (index < 0) {
            throw new ArrayIndexOutOfBoundsException("Slice index must be non-negative.");
        } else if (index >= cap) {
            throw new ArrayIndexOutOfBoundsException("Index must be less than the capacity of the slice.");
        } else if (index >= len) {
            throw new ArrayIndexOutOfBoundsException("Index must be less than the length of the slice.");
        } else {
            elementList.add(index, element);
        }
    }
}// Utility class containing all methods to cast between types

class Cast {
 public static Integer castToInteger(Integer i) {
  return i;
 }

 public static Integer castToInteger(Double d) {
  return d.intValue(); 
 }

 public static Integer castToInteger(Character c) {
  return (int) c;
 }

 public static Double castToDouble(Integer i) {
  return i.doubleValue();
 }

 public static Double castToDouble(Double d) {
  return d;
 }

 public static Double castToDouble(Character c) {
  return (double) c;
 }

 public static String castToString(Integer i) {
  return new Character((char) i.intValue()).toString();
 }

 public static String castToString(String s) {
  return s;
 }

 public static String castToString(Character c) {
  return c.toString();
 }

 public static Character castToCharacter(Integer i) {
  return new Character((char) i.intValue());
 }

 public static Character castToCharacter(Double d) {
  return new Character((char) d.intValue());
 }

 public static Character castToCharacter(Character c) {
  return c;
 }
}
public class indexingexprGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__main () {
		
		Slice<Integer> __golite__ws_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__xs_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__ys_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__zs_1 = new Slice<Integer>()
		
		Integer[] __golite__wa_1 = new Integer[5]
		
		Integer[] __golite__xa_1 = new Integer[5]
		
		Integer[] __golite__ya_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__ya_1[_golite_iter_i0] = new Integer(0);
		}

		
		Integer[] __golite__za_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__za_1[_golite_iter_i0] = new Integer(0);
		}

		
		Integer __golite__a_1 = new Integer(0)
		
		Integer __golite__e_1 = new Integer(0)
		
		Integer __golite__i_1 = new Integer(0)
		
		Integer __golite__a_1_temp_assign_0 = __golite__ws_1.get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_0;
		Integer __golite__a_1_temp_assign_1 = __golite__xs_1.get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_1;
		Integer __golite__a_1_temp_assign_2 = __golite__ys_1.get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_2;
		Integer __golite__e_1_temp_assign_3 = __golite__zs_1.get(new Integer(0));
		__golite__e_1 = __golite__e_1_temp_assign_3;
		Integer __golite__a_1_temp_assign_4 = __golite__ws_1.get(__golite__i_1);
		__golite__a_1 = __golite__a_1_temp_assign_4;
		Integer __golite__a_1_temp_assign_5 = __golite__xs_1.get(__golite__i_1);
		__golite__a_1 = __golite__a_1_temp_assign_5;
		Integer __golite__a_1_temp_assign_6 = __golite__ys_1.get(__golite__i_1);
		__golite__a_1 = __golite__a_1_temp_assign_6;
		Integer __golite__e_1_temp_assign_7 = __golite__zs_1.get(__golite__i_1);
		__golite__e_1 = __golite__e_1_temp_assign_7;
		Integer __golite__a_1_temp_assign_8 = __golite__wa_1[new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_8;
		Integer __golite__a_1_temp_assign_9 = __golite__xa_1[new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_9;
		Integer __golite__a_1_temp_assign_10 = __golite__ya_1[new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_10;
		Integer __golite__e_1_temp_assign_11 = __golite__za_1[new Integer(0)];
		__golite__e_1 = __golite__e_1_temp_assign_11;
		Integer __golite__a_1_temp_assign_12 = __golite__wa_1[__golite__i_1];
		__golite__a_1 = __golite__a_1_temp_assign_12;
		Integer __golite__a_1_temp_assign_13 = __golite__xa_1[__golite__i_1];
		__golite__a_1 = __golite__a_1_temp_assign_13;
		Integer __golite__a_1_temp_assign_14 = __golite__ya_1[__golite__i_1];
		__golite__a_1 = __golite__a_1_temp_assign_14;
		Integer __golite__e_1_temp_assign_15 = __golite__za_1[__golite__i_1];
		__golite__e_1 = __golite__e_1_temp_assign_15;
		Slice<Slice<Integer>> __golite__wsm_1 = new Slice<Slice<Integer>>()
		
		Slice<Slice<Integer>> __golite__xsm_1 = new Slice<Slice<Integer>>()
		
		Slice<Slice<Integer>> __golite__ysm_1 = new Slice<Slice<Integer>>()
		
		Slice<Slice<Integer>> __golite__zsm_1 = new Slice<Slice<Integer>>()
		
		Slice<Integer[]> __golite__wam_1 = new Slice<Integer[]>()
		
		Slice<Integer[]> __golite__xam_1 = new Slice<Integer[]>()
		
		Integer[][] __golite__yam_1 = new Integer[5][5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			for(int _golite_iter_i1 = 0; _golite_iter_i1 < 5; _golite_iter_i1++){
				__golite__yam_1[_golite_iter_i1][_golite_iter_i0] = new Integer(0);
			}
		}

		
		Integer[][] __golite__zam_1 = new Integer[5][5];
		for(int _golite_iter_i1 = 0; _golite_iter_i1 < 5; _golite_iter_i1++){
			for(int _golite_iter_i2 = 0; _golite_iter_i2 < 5; _golite_iter_i2++){
				__golite__zam_1[_golite_iter_i2][_golite_iter_i1] = new Integer(0);
			}
		}

		
		Slice<Integer> __golite__ism_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__dsm_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__asm_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__esm_1 = new Slice<Integer>()
		
		Integer[] __golite__iam_1 = new Integer[5]
		
		Integer[] __golite__dam_1 = new Integer[5]
		
		Integer[] __golite__aam_1 = new Integer[5];
		for(int _golite_iter_i2 = 0; _golite_iter_i2 < 5; _golite_iter_i2++){
			__golite__aam_1[_golite_iter_i2] = new Integer(0);
		}

		
		Integer[] __golite__eam_1 = new Integer[5];
		for(int _golite_iter_i2 = 0; _golite_iter_i2 < 5; _golite_iter_i2++){
			__golite__eam_1[_golite_iter_i2] = new Integer(0);
		}

		
		Slice<Integer> __golite__ism_1_temp_assign_16 = __golite__wsm_1.get(new Integer(0));
		__golite__ism_1 = __golite__ism_1_temp_assign_16;
		Slice<Integer> __golite__dsm_1_temp_assign_17 = __golite__xsm_1.get(new Integer(0));
		__golite__dsm_1 = __golite__dsm_1_temp_assign_17;
		Slice<Integer> __golite__asm_1_temp_assign_18 = __golite__ysm_1.get(new Integer(0));
		__golite__asm_1 = __golite__asm_1_temp_assign_18;
		Slice<Integer> __golite__esm_1_temp_assign_19 = __golite__zsm_1.get(new Integer(0));
		__golite__esm_1 = __golite__esm_1_temp_assign_19;
		Integer __golite__a_1_temp_assign_20 = __golite__wsm_1.get(new Integer(0)).get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_20;
		Integer __golite__a_1_temp_assign_21 = __golite__xsm_1.get(new Integer(0)).get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_21;
		Integer __golite__a_1_temp_assign_22 = __golite__ysm_1.get(new Integer(0)).get(new Integer(0));
		__golite__a_1 = __golite__a_1_temp_assign_22;
		Integer __golite__e_1_temp_assign_23 = __golite__zsm_1.get(new Integer(0)).get(new Integer(0));
		__golite__e_1 = __golite__e_1_temp_assign_23;
		Integer[] __golite__iam_1_temp_assign_24 = __golite__wam_1.get(new Integer(0));
		__golite__iam_1 = __golite__iam_1_temp_assign_24;
		Integer[] __golite__dam_1_temp_assign_25 = __golite__xam_1.get(new Integer(0));
		__golite__dam_1 = __golite__dam_1_temp_assign_25;
		Integer[] __golite__aam_1_temp_assign_26 = __golite__yam_1[new Integer(0)];
		__golite__aam_1 = __golite__aam_1_temp_assign_26;
		Integer[] __golite__eam_1_temp_assign_27 = __golite__zam_1[new Integer(0)];
		__golite__eam_1 = __golite__eam_1_temp_assign_27;
		Integer __golite__a_1_temp_assign_28 = __golite__wam_1.get(new Integer(0))[new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_28;
		Integer __golite__a_1_temp_assign_29 = __golite__xam_1.get(new Integer(0))[new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_29;
		Integer __golite__a_1_temp_assign_30 = __golite__yam_1[new Integer(0)][new Integer(0)];
		__golite__a_1 = __golite__a_1_temp_assign_30;
		Integer __golite__e_1_temp_assign_31 = __golite__zam_1[new Integer(0)][new Integer(0)];
		__golite__e_1 = __golite__e_1_temp_assign_31;
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
