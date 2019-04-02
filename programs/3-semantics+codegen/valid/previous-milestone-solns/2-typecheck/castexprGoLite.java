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
public class castexprGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__cast_int () {
		
		Integer __golite__x_1 = new Integer(new Integer(5));
		
		Integer __golite__y_1 = new Integer(castUtil.castToInteger(__golite__x_1));
		
		Integer __golite__x_1_temp_assign_0 = castUtil.castToInteger(__golite__y_1);
		__golite__x_1 = __golite__x_1_temp_assign_0;
	}
	public static void __golite__cast_float64 () {
		
		Double __golite__x_1 = new Double(new Double(5.000000));
		
		Double __golite__y_1 = new Double(castUtil.castToDouble(__golite__x_1));
		
		Double __golite__x_1_temp_assign_1 = castUtil.castToDouble(__golite__y_1);
		__golite__x_1 = __golite__x_1_temp_assign_1;
	}
	public static void __golite__cast_rune () {
		
		Character __golite__x_1 = new Character(new Character('z'));
		
		Character __golite__y_1 = new Character(castUtil.castToCharacter(__golite__x_1));
		
		Character __golite__x_1_temp_assign_2 = castUtil.castToCharacter(__golite__y_1);
		__golite__x_1 = __golite__x_1_temp_assign_2;
	}
	public static void __golite__cast_bool () {
		
		Boolean __golite__x_1 = new Boolean(__golite__true);
		
		Boolean __golite__y_1 = new Boolean();
		
		Boolean __golite__x_1_temp_assign_3 = ;
		__golite__x_1 = __golite__x_1_temp_assign_3;
	}
	public static void __golite__cast_string () {
		
		String __golite__x_1 = new String("");
		
		String __golite__y_1 = new String(castUtil.castToString(__golite__x_1));
		
		String __golite__x_1_temp_assign_4 = castUtil.castToString(__golite__y_1);
		__golite__x_1 = __golite__x_1_temp_assign_4;
	}
	public static void __golite__cast_underlying () {
		
		Integer __golite__a_1 = new Integer(0)
		
		Integer __golite__b_1 = new Integer(0)
		
		Integer __golite__a_1_temp_assign_5 = castUtil.castToInteger(__golite__b_1);
		__golite__a_1 = __golite__a_1_temp_assign_5;
		Integer __golite__b_1_temp_assign_6 = castUtil.castToInteger(__golite__a_1);
		__golite__b_1 = __golite__b_1_temp_assign_6;
	}
	public static void __golite__cast_int_string () {
		
		Integer __golite__a_1 = new Integer(0)
		
		String __golite__b_1 = new String("")
		
		Character __golite__c_1 = new Character(' ')
		
		String __golite__b_1_temp_assign_7 = castUtil.castToString(__golite__a_1);
		__golite__b_1 = __golite__b_1_temp_assign_7;
		String __golite__b_1_temp_assign_8 = castUtil.castToString(__golite__c_1);
		__golite__b_1 = __golite__b_1_temp_assign_8;
	}
	public static void __golite__cast_int_float () {
		
		Integer __golite__a_1 = new Integer(0)
		
		Double __golite__b_1 = new Double(0.0)
		
		Character __golite__c_1 = new Character(' ')
		
		Integer __golite__a_1_temp_assign_9 = castUtil.castToInteger(__golite__b_1);
		__golite__a_1 = __golite__a_1_temp_assign_9;
		Integer __golite__a_1_temp_assign_10 = castUtil.castToInteger(__golite__c_1);
		__golite__a_1 = __golite__a_1_temp_assign_10;
		Double __golite__b_1_temp_assign_11 = castUtil.castToDouble(__golite__a_1);
		__golite__b_1 = __golite__b_1_temp_assign_11;
		Double __golite__b_1_temp_assign_12 = castUtil.castToDouble(__golite__c_1);
		__golite__b_1 = __golite__b_1_temp_assign_12;
		Character __golite__c_1_temp_assign_13 = castUtil.castToCharacter(__golite__a_1);
		__golite__c_1 = __golite__c_1_temp_assign_13;
		Character __golite__c_1_temp_assign_14 = castUtil.castToCharacter(__golite__b_1);
		__golite__c_1 = __golite__c_1_temp_assign_14;
		Integer __golite__d_1 = new Integer(0)
		
		Double __golite__e_1 = new Double(0.0)
		
		Character __golite__f_1 = new Character(' ')
		
		Integer __golite__d_1_temp_assign_15 = castUtil.castToInteger(__golite__e_1);
		__golite__d_1 = __golite__d_1_temp_assign_15;
		Integer __golite__d_1_temp_assign_16 = castUtil.castToInteger(__golite__f_1);
		__golite__d_1 = __golite__d_1_temp_assign_16;
		Double __golite__e_1_temp_assign_17 = castUtil.castToDouble(__golite__d_1);
		__golite__e_1 = __golite__e_1_temp_assign_17;
		Double __golite__e_1_temp_assign_18 = castUtil.castToDouble(__golite__f_1);
		__golite__e_1 = __golite__e_1_temp_assign_18;
		Character __golite__f_1_temp_assign_19 = castUtil.castToCharacter(__golite__d_1);
		__golite__f_1 = __golite__f_1_temp_assign_19;
		Character __golite__f_1_temp_assign_20 = castUtil.castToCharacter(__golite__e_1);
		__golite__f_1 = __golite__f_1_temp_assign_20;
	}
	public static void __golite__main () {
		
		__golite__cast_int();
		
		__golite__cast_float64();
		
		__golite__cast_rune();
		
		__golite__cast_bool();
		
		__golite__cast_string();
		
		__golite__cast_underlying();
		
		__golite__cast_int_string();
		
		__golite__cast_int_float();
		
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
