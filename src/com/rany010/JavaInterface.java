package com.rany010;
import java.util.LinkedList;
import java.util.List;

import com.sun.org.apache.xpath.internal.jaxp.JAXPVariableStack;
import com.sun.xml.internal.ws.wsdl.writer.document.soap.Body;
import sun.security.krb5.internal.PAEncTSEnc;

public class JavaInterface {
    static {
        System.loadLibrary("cppInterface");
    }

    public native static void testFunc();
    public native static String inputAndOutputString(String str);
    public native static boolean setStruct(Params params);
    public native static Params getStruct();
    public native static boolean setListStruct(List<Params> paramsInput);
    public native static List<Params> getListStruct();
    public native static boolean setStructList(Params paramsInput);

    public static void main(String[] args) {
        System.out.println("[JAVA] start java");

        JavaInterface.testFunc();

        System.out.println("[JAVA] "+JavaInterface.inputAndOutputString("hello cpp"));

        Params paramsInput = new Params();
        paramsInput.id = 01;
        paramsInput.name = "java";
        System.out.println("[JAVA] "+JavaInterface.setStruct(paramsInput));

        Params outParams = JavaInterface.getStruct();
        System.out.print("[JAVA] getStruct id: "+outParams.id);
        System.out.println(" name: "+outParams.name);

        List<Params> paramsList = new LinkedList<>();
        Params p1 = new Params();
        p1.id = 88;
        p1.name = "java 88";
        Params p2 = new Params();
        p2.id = 99;
        p2.name = "java 99";
        paramsList.add(p1);
        paramsList.add(p2);
        System.out.println("[JAVA] "+JavaInterface.setListStruct(paramsList));

        List<Params> outListParams = JavaInterface.getListStruct();
        for(Params p : outListParams){
            System.out.print("[JAVA] id: "+p.id);
            System.out.println(" name: "+p.name);
        }


        Params inParams = new Params();
        inParams.id = 67;
        inParams.name = "java 67";
        BodyIndex bodyIndex1 = new BodyIndex();
        bodyIndex1.sex = true;
        bodyIndex1.height = 6;
        bodyIndex1.wight = 6;

        BodyIndex bodyIndex2 = new BodyIndex();
        bodyIndex2.sex = true;
        bodyIndex2.height = 7;
        bodyIndex2.wight = 7;

        List<BodyIndex> bodyList = new LinkedList<>();
        bodyList.add(bodyIndex1);
        bodyList.add(bodyIndex2);

        inParams.body = bodyList;
        System.out.println("[JAVA] "+JavaInterface.setStructList(inParams));

    }
}
