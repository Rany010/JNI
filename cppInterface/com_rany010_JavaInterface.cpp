#include <jni.h>
#include <com_rany010_JavaInterface.h>
#include <iostream>

#include <cppinterface.h>

JNIEXPORT void JNICALL Java_com_rany010_JavaInterface_testFunc
(JNIEnv *, jobject){
    std::cout<<"[CPP] JNI testFunc hello java"<<std::endl;
}

JNIEXPORT jstring JNICALL Java_com_rany010_JavaInterface_inputAndOutputString
(JNIEnv *env, jobject, jstring strInput){
    const char* str = env->GetStringUTFChars(strInput, nullptr);
    if(str == nullptr)
    {
        std::cout<<"[CPP] inputAndOutputString,GetStringUTFChars fail"<<std::endl;
        return nullptr;
    }

    return env->NewStringUTF(str);
}

JNIEXPORT jboolean JNICALL Java_com_rany010_JavaInterface_setStruct
(JNIEnv *env, jobject, jobject paramsInput){

    //得到类
    jclass jcs = env->GetObjectClass(paramsInput);

    //通过id从类获得字段id
    jfieldID idId = env->GetFieldID(jcs, "id", "I");
    jfieldID nameId = env->GetFieldID(jcs, "name", "Ljava/lang/String;");

    //通过字段id从传入的结构中得到数据
    int id = env->GetIntField(paramsInput, idId);
    jstring jStrName = static_cast<jstring>(env->GetObjectField(paramsInput, nameId));
    std::string strName = env->GetStringUTFChars(jStrName, nullptr);

    std::cout<<"[CPP] setStruct, input struct id: "<<id<<" name: "<<strName.c_str()<<std::endl;

    return true;
}

JNIEXPORT jobject JNICALL Java_com_rany010_JavaInterface_getStruct
(JNIEnv *env, jobject){
    //cpp端需要对应建立参数结构
    Params params;
    params.id = 2;
    params.name = "cpp";

    //映射到java端定义参数类的位置
    jclass jcs = env->FindClass("com/rany010/Params");
    //得到字段id
    jfieldID idId = env->GetFieldID(jcs, "id", "I");
    jfieldID nameId = env->GetFieldID(jcs, "name", "Ljava/lang/String;");

    //根据从java端得到的class创建一个oibject，用于后续为其设置参数值
    jobject paramsJobj = env->AllocObject(jcs);

    //向obj中指定字段设置内容
    env->SetIntField(paramsJobj, idId, params.id);
    env->SetObjectField(paramsJobj, nameId, env->NewStringUTF(params.name.c_str()));

    return paramsJobj;
}

JNIEXPORT jboolean JNICALL Java_com_rany010_JavaInterface_setListStruct
  (JNIEnv *env, jobject, jobject paramsInput){
    //获得ListObject
    jclass listCls = env->GetObjectClass(paramsInput);

    //得到方法id
    jmethodID getId = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");
    jmethodID sizeId = env->GetMethodID(listCls, "size", "()I");

    //根据sizeId得到list的size
    int listSize = env->CallIntMethod(paramsInput, sizeId);

    //遍历
    for(int i=0;i<listSize;i++)
    {
        //得到struct对象
        jobject paramsObj = env->CallObjectMethod(paramsInput, getId, i);
        //获取params类
        jclass paramsCls = env->GetObjectClass(paramsObj);

        //获得struct内实现的get方法的id
        jmethodID idMethodId = env->GetMethodID(paramsCls, "getId", "()I");
        jmethodID nameMethodId = env->GetMethodID(paramsCls, "getName", "()Ljava/lang/String;");

        //获得字段值
        jint idValue = env->CallIntMethod(paramsObj, idMethodId);
        jstring nameValue = static_cast<jstring>(env->CallObjectMethod(paramsObj, nameMethodId));

        std::cout<<"[CPP] setStructList index: "<<i<<" id value: "<<idValue<<" name value: "<<env->GetStringUTFChars(nameValue, nullptr)<<std::endl;
    }

    return true;
}

JNIEXPORT jobject JNICALL Java_com_rany010_JavaInterface_getListStruct
(JNIEnv *env, jobject){
    //cpp端需要组织数据
    std::list<Params> listParams;
    Params p1;
    p1.id = 88;
    p1.name = "cpp 88";
    Params p2;
    p2.id = 99;
    p2.name = "cpp 99";
    listParams.push_back(p1);
    listParams.push_back(p2);

    //获取一个java的list类
    jclass listCls = env->FindClass("java/util/ArrayList");
    //获取list的构造函数id
    jmethodID initId = env->GetMethodID(listCls, "<init>", "()V");
    //获取list的add方法id
    jmethodID addId = env->GetMethodID(listCls, "add","(Ljava/lang/Object;)Z");
    //创建一个list对象
    jobject listObj = env->NewObject(listCls, initId, "");

    //开始组装数据
    for(auto iter = listParams.begin();iter!=listParams.end();iter++){
        Params tmpParams = *iter;

        //获取Params类
        jclass paramsCls = env->FindClass("com/rany010/Params");
        //获取Params指定字段id
        jfieldID idId = env->GetFieldID(paramsCls, "id", "I");
        jfieldID nameId = env->GetFieldID(paramsCls, "name", "Ljava/lang/String;");

        //通过params的类得到一个obj,将数据通过字段id设置进去
        jobject paramsObj = env->AllocObject(paramsCls);
        env->SetIntField(paramsObj, idId, tmpParams.id);
        env->SetObjectField(paramsObj, nameId, env->NewStringUTF(tmpParams.name.c_str()));

        //将每个组装好数据的Params obj借助list的addid添加到list后返回
        env->CallBooleanMethod(listObj,addId, paramsObj);
    }

    return listObj;
}

JNIEXPORT jboolean JNICALL Java_com_rany010_JavaInterface_setStructList
(JNIEnv *env, jobject, jobject paramsInput){
    Params params;

    jclass paramsCls = env->GetObjectClass(paramsInput);

    //获得字段id
    jfieldID idId = env->GetFieldID(paramsCls, "id", "I");
    jfieldID nameId = env->GetFieldID(paramsCls, "name", "Ljava/lang/String;");
    jfieldID bodyListId = env->GetFieldID(paramsCls, "body", "Ljava/util/List;");

    //根据id获得值
    jint idValue = env->GetIntField(paramsInput, idId);
    jstring nameValue = static_cast<jstring>(env->GetObjectField(paramsInput, nameId));

    params.id = idValue;
    params.name = env->GetStringUTFChars(nameValue,nullptr);

    //通过bodyList的id获得obj
    jobject bodyListObj = env->GetObjectField(paramsInput, bodyListId);
    jclass bodyListCls = env->GetObjectClass(bodyListObj);
    //获得list类对应的get和size方法id
    jmethodID bodyListGetId = env->GetMethodID(bodyListCls, "get", "(I)Ljava/lang/Object;");
    jmethodID bodyListSizeId = env->GetMethodID(bodyListCls, "size", "()I");

    //获取传入的list的size然后遍历
    jint bodyListSize = env->CallIntMethod(bodyListObj, bodyListSizeId);
    for(int i=0;i<bodyListSize;i++){
        //定义结构体对象，将传入的数据写入其中
        BodyIndex bodyIndex;

        jobject bodyIndexObj = env->CallObjectMethod(bodyListObj, bodyListGetId, i);
        jclass bodyIndexCls = env->GetObjectClass(bodyIndexObj);

        //得到BodyIndex类字段get方法id
        jmethodID bodyIndexGetSexId = env->GetMethodID(bodyIndexCls, "getSex", "()Z");
        jmethodID bodyIndexGetHeightID = env->GetMethodID(bodyIndexCls, "getHeight", "()F");
        jmethodID bodyIndexGetWightId = env->GetMethodID(bodyIndexCls, "getWight", "()D");

        bodyIndex.sex = env->CallBooleanMethod(bodyIndexObj, bodyIndexGetSexId);
        bodyIndex.height = env->CallFloatMethod(bodyIndexObj, bodyIndexGetHeightID);
        bodyIndex.wight = env->CallDoubleMethod(bodyIndexObj, bodyIndexGetWightId);

        params.body.push_back(bodyIndex);
    }

    std::cout<<"[CPP] setStructList id: "<<params.id<<" name: "<<params.name.c_str()<<std::endl;
    for(BodyIndex vue : params.body){
        std::cout<<"sex: "<<vue.sex<<" height: "<<vue.height<<" wight: "<<vue.wight<<std::endl;
    }

    return true;
}
