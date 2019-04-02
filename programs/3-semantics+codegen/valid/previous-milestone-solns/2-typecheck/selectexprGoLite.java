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
class STRUCT1 {
	STRUCT0 i = new STRUCT0();
	STRUCT0 j = new STRUCT0();
	public Boolean equals(STRUCT1 other){
		return this.i.equals(other.i) && this.j.equals(other.j);
	}
}
class STRUCT0 {
	Double x = new Double(0.0);
	Double y = new Double(0.0);
	Double z = new Double(0.0);
	public Boolean equals(STRUCT0 other){
		return this.x.equals(other.x) && this.y.equals(other.y) && this.z.equals(other.z);
	}
}
class STRUCT2 {
	Boolean a = new Boolean(false);
	Integer b = new Integer(0);
	String c = new String("");
	public Boolean equals(STRUCT2 other){
		return this.a.equals(other.a) && this.b.equals(other.b) && this.c.equals(other.c);
	}
}
public class selectexprGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__main () {
		
		Boolean __golite__a_1 = new Boolean(false)
		
		Integer __golite__b_1 = new Integer(0)
		
		String __golite__c_1 = new String("")
		
		STRUCT2 __golite__s_1 = new STRUCT2()
		
		__golite__s_1.a = __golite__true;
		__golite__s_1.b = new Integer(42);
		__golite__s_1.c = "struct";
		Boolean __golite__a_1_temp_assign_0 = __golite__s_1.a;
		__golite__a_1 = __golite__a_1_temp_assign_0;
		Integer __golite__b_1_temp_assign_1 = __golite__s_1.b;
		__golite__b_1 = __golite__b_1_temp_assign_1;
		String __golite__c_1_temp_assign_2 = __golite__s_1.c;
		__golite__c_1 = __golite__c_1_temp_assign_2;
		Double __golite__x_1 = new Double(0.0)
		Double __golite__y_1 = new Double(0.0)
		Double __golite__z_1 = new Double(0.0)
		
		STRUCT0 __golite__p_1 = new STRUCT0()
		
		__golite__p_1.x = new Double(0.000000);
		__golite__p_1.y = new Double(-new Double(1.000000));
		__golite__p_1.z = new Double(1.000000);
		Double __golite__x_1_temp_assign_3 = __golite__p_1.x;
		__golite__x_1 = __golite__x_1_temp_assign_3;
		Double __golite__y_1_temp_assign_4 = __golite__p_1.y;
		__golite__y_1 = __golite__y_1_temp_assign_4;
		Double __golite__x_1_temp_assign_5 = __golite__p_1.z;
		__golite__x_1 = __golite__x_1_temp_assign_5;
		STRUCT0 __golite__q_1 = new STRUCT0()
		
		__golite__q_1.x = new Double(0.000000);
		__golite__q_1.y = new Double(-new Double(1.000000));
		__golite__q_1.z = new Double(1.000000);
		Double __golite__x_1_temp_assign_6 = __golite__q_1.x;
		__golite__x_1 = __golite__x_1_temp_assign_6;
		Double __golite__y_1_temp_assign_7 = __golite__q_1.y;
		__golite__y_1 = __golite__y_1_temp_assign_7;
		Double __golite__z_1_temp_assign_8 = __golite__q_1.z;
		__golite__z_1 = __golite__z_1_temp_assign_8;
		STRUCT1 __golite__v_1 = new STRUCT1()
		
		__golite__v_1.i = __golite__q_1;
		__golite__v_1.j = __golite__q_1;
		__golite__v_1.i.x = new Double(2.000000);
		__golite__v_1.i.y = new Double(3.000000);
		__golite__v_1.i.z = new Double(4.000000);
		Double __golite__x_1_temp_assign_9 = __golite__v_1.i.x;
		__golite__x_1 = __golite__x_1_temp_assign_9;
		Double __golite__y_1_temp_assign_10 = __golite__v_1.i.y;
		__golite__y_1 = __golite__y_1_temp_assign_10;
		Double __golite__z_1_temp_assign_11 = __golite__v_1.i.z;
		__golite__z_1 = __golite__z_1_temp_assign_11;
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
