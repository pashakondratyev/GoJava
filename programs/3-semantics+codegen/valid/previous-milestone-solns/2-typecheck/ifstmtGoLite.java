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
public class ifstmtGoLite {
	public static Boolean __golite__true = Boolean.TRUE;
	public static Boolean __golite__false = Boolean.FALSE;

	public static Cast castUtil = new Cast();
		public static void __golite__main () {
		
		Boolean __golite__my_bool_1 = new Boolean(false)
		
		Boolean __golite__my_t_1 = new Boolean(false)
		
		{
			if (__golite__my_bool_1){
				}
		}
		
		{
			if (__golite__my_bool_1){
				}else{
					}				
		}
		
		{
			if (__golite__my_bool_1){
				}else{
						if (__golite__my_bool_1){
							}else{
								}							
					}
									
		}
		
		{
			if (__golite__my_t_1){
				}
		}
		
		{
			if (__golite__my_t_1){
				}else{
					}				
		}
		
		{
			if (__golite__my_t_1){
				}else{
						if (__golite__my_t_1){
							}else{
								}							
					}
									
		}
		
		{
			if (__golite__my_t_1){
				}else{
						if (__golite__my_bool_1){
							}
					}
									
		}
		
		{
			Integer __golite__x_temp_1 = new Integer(1);
			Integer __golite__x_1 = __golite__x_temp_1;
			if (new Boolean(__golite__x_1.equals(new Integer(0)))){
					System.out.print(__golite__x_1);
					System.out.println();
					
				}else{
						Integer __golite__y_temp_1 = new Integer(2);
						Integer __golite__y_1 = __golite__y_temp_1;
						if (new Boolean(__golite__x_1.equals(new Integer(1)))){
								System.out.print(__golite__x_1);
								System.out.print(" ");
								System.out.print(__golite__y_1);
								System.out.println();
								
							}else{
									System.out.print(__golite__x_1);
									System.out.print(" ");
									System.out.print(__golite__y_1);
									System.out.println();
									
								}							
					}
									
		}
		
	}
	public static void main(String[] args) {
		__golite__main();
	}
}
