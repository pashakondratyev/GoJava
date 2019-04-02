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
	Double x = new Double(0.0);
	Double y = new Double(0.0);
	Double z = new Double(0.0);
	public Boolean equals(STRUCT0 other){
		return this.x.equals(other.x) && this.y.equals(other.y) && this.z.equals(other.z);
	}
}
class STRUCT1 {
	Double k = new Double(0.0);
	STRUCT2 n = new STRUCT2();
	public Boolean equals(STRUCT1 other){
		return this.k.equals(other.k) && this.n.equals(other.n);
	}
}
class STRUCT2 {
	Integer a = new Integer(0);
	public Boolean equals(STRUCT2 other){
		return this.a.equals(other.a);
	}
}
class STRUCT3 {
	public Boolean equals(STRUCT3 other){
		return ;
	}
}
public class decstmtsGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__dec_stmts () {
		
		Integer __golite__a1_1 = new Integer(0)
		
		Double __golite__a2_1 = new Double(new Double(0.000000));
		
		Double __golite__a3_1 = new Double(new Double(0.000000));
		
		String __golite__a4_1 = new String("")
		String __golite__a5_1 = new String("")
		
		String __golite__a6_1 = new String("a");
		String __golite__a7_1 = new String(`b`);
		
		Character __golite__a8_1 = new Character(new Character('a'));
		Integer __golite__a9_1 = new Integer(new Integer(0));
		
		Integer __golite__blank_0 = new Integer(0)
		
		Integer __golite__b1_1 = new Integer(0)
		Double __golite__b2_1 = new Double(new Double(0.000000));
		Double __golite__b3_1 = new Double(new Double(0.000000));
		String __golite__b4_1 = new String("")
		String __golite__b5_1 = new String("")
		String __golite__b6_1 = new String("a");
		String __golite__b7_1 = new String(`b`);
		Character __golite__b8_1 = new Character(new Character('a'));
		Integer __golite__b9_1 = new Integer(new Integer(0));
		Integer __golite__blank_1 = new Integer(0)
		
		
	}
	public static void __golite__main () {
		
		__golite__dec_stmts();
		
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
