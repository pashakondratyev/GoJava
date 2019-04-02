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
class STRUCT0 {
	Integer x = new Integer(0);
	public Boolean equals(STRUCT0 other){
		return this.x.equals(other.x);
	}
}
class STRUCT1 {
	Integer x = new Integer(0);
	Integer y = new Integer(0);
	public Boolean equals(STRUCT1 other){
		return this.x.equals(other.x) && this.y.equals(other.y);
	}
}
public class assignstmtsGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__assign_int () {
		
		Integer __golite__a_1 = new Integer(0)
		Integer __golite__b_1 = new Integer(0)
		
		Integer __golite__a_1_temp_assign_0 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_0;
		Integer __golite__c_1 = new Integer(0)
		Integer __golite__d_1 = new Integer(0)
		
		Integer __golite__c_1_temp_assign_1 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_1;
	}
	public static void __golite__assign_rune () {
		
		Character __golite__a_1 = new Character(' ')
		Character __golite__b_1 = new Character(' ')
		
		Character __golite__a_1_temp_assign_2 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_2;
		Character __golite__c_1 = new Character(' ')
		Character __golite__d_1 = new Character(' ')
		
		Character __golite__c_1_temp_assign_3 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_3;
	}
	public static void __golite__assign_float64 () {
		
		Double __golite__a_1 = new Double(0.0)
		Double __golite__b_1 = new Double(0.0)
		
		Double __golite__a_1_temp_assign_4 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_4;
		Double __golite__c_1 = new Double(0.0)
		Double __golite__d_1 = new Double(0.0)
		
		Double __golite__c_1_temp_assign_5 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_5;
	}
	public static void __golite__assign_bool () {
		
		Boolean __golite__a_1 = new Boolean(false)
		Boolean __golite__b_1 = new Boolean(false)
		
		Boolean __golite__a_1_temp_assign_6 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_6;
		Boolean __golite__c_1 = new Boolean(false)
		Boolean __golite__d_1 = new Boolean(false)
		
		Boolean __golite__c_1_temp_assign_7 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_7;
	}
	public static void __golite__assign_string () {
		
		String __golite__a_1 = new String("")
		String __golite__b_1 = new String("")
		
		String __golite__a_1_temp_assign_8 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_8;
		String __golite__c_1 = new String("")
		String __golite__d_1 = new String("")
		
		String __golite__c_1_temp_assign_9 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_9;
	}
	public static void __golite__assign_slice () {
		
		Slice<Integer> __golite__a_1 = new Slice<Integer>()
		Slice<Integer> __golite__b_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__a_1_temp_assign_10 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_10;
		Slice<Integer> __golite__c_1 = new Slice<Integer>()
		Slice<Integer> __golite__d_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__c_1_temp_assign_11 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_11;
		Slice<Integer> __golite__e_1 = new Slice<Integer>()
		Slice<Integer> __golite__f_1 = new Slice<Integer>()
		
		Slice<Integer> __golite__e_1_temp_assign_12 = __golite__f_1;
		__golite__e_1 = __golite__e_1_temp_assign_12;
	}
	public static void __golite__assign_array () {
		
		Integer[] __golite__a_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__a_1[_golite_iter_i0] = new Integer(0);
		}

		Integer[] __golite__b_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__b_1[_golite_iter_i0] = new Integer(0);
		}

		
		Integer[] __golite__a_1_temp_assign_13 = __golite__b_1;
		__golite__a_1 = __golite__a_1_temp_assign_13;
		Integer[] __golite__c_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__c_1[_golite_iter_i0] = new Integer(0);
		}

		Integer[] __golite__d_1 = new Integer[5];
		for(int _golite_iter_i0 = 0; _golite_iter_i0 < 5; _golite_iter_i0++){
			__golite__d_1[_golite_iter_i0] = new Integer(0);
		}

		
		Integer[] __golite__c_1_temp_assign_14 = __golite__d_1;
		__golite__c_1 = __golite__c_1_temp_assign_14;
		Integer[] __golite__e_1