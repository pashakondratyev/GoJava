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
public class returnstmtGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static Double __golite__incr (Double __golite__x_1) {
		
		return new Double(__golite__x_1 + new Double(1.000000));
	}
	public static STRUCT0 __golite__new_point () {
		
		STRUCT0 __golite__p_1 = new STRUCT0()
		
		__golite__p_1.x = __golite__incr(new Double(-new Double(1.000000)));
		__golite__p_1.y = new Double(0.000000);
		__golite__p_1.z = new Double(0.000000);
		return __golite__p_1;
	}
	public static void __golite__main () {
		
		return ;
	}
	public static Integer __golite__ret_loop_3part () {
		
		{
			
			while (){
				System.out.print(new Integer(0));
			}
		}
	}
	public static Integer __golite__ret_loop_inf () {
		
		while (true) {
				System.out.print(new Integer(0));
			}
		
	}
	public static Integer __golite__ret_if () {
		
		{
			if (__golite__true){
					return new Integer(0);
				}else{
						if (__golite__false){
								return new Integer(1);
							}else{
									return new Integer(2);
								}							
					}
									
		}
		
	}
	public static Integer __golite__ret_switch () {
		
		{
			while (true) {
				Boolean switchCond_0 = Boolean.TRUE;
				if (switchCond_0.equals(__golite__true)) {
				return new Integer(0);} if (switchCond_0.equals(__golite__false)) {
				return new Integer(1);} else { 
				return new Integer(2);}
			break;
			}
		}
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
