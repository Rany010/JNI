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

    //�õ���
    jclass jcs = env->GetObjectClass(paramsInput);

    //ͨ��id�������ֶ�id
    jfieldID idId = env->GetFieldID(jcs, "id", "I");
    jfieldID nameId = env->GetFieldID(jcs, "name", "Ljava/lang/String;");

    //ͨ���ֶ�id�Ӵ���Ľṹ�еõ�����
    int id = env->GetIntField(paramsInput, idId);
    jstring jStrName = static_cast<jstring>(env->GetObjectField(paramsInput, nameId));
    std::string strName = env->GetStringUTFChars(jStrName, nullptr);

    std::cout<<"[CPP] setStruct, input struct id: "<<id<<" name: "<<strName.c_str()<<std::endl;

    return true;
}

JNIEXPORT jobject JNICALL Java_com_rany010_JavaInterface_getStruct
(JNIEnv *env, jobject){
    //cpp����Ҫ��Ӧ���������ṹ
    Params params;
    params.id = 2;
    params.name = "cpp";

    //ӳ�䵽java�˶���������λ��
    jclass jcs = env->FindClass("com/rany010/Params");
    //�õ��ֶ�id
    jfieldID idId = env->GetFieldID(jcs, "id", "I");
    jfieldID nameId = env->GetFieldID(jcs, "name", "Ljava/lang/String;");

    //���ݴ�java�˵õ���class����һ��oibject�����ں���Ϊ�����ò���ֵ
    jobject paramsJobj = env->AllocObject(jcs);

    //��obj��ָ���ֶ���������
    env->SetIntField(paramsJobj, idId, params.id);
    env->SetObjectField(paramsJobj, nameId, env->NewStringUTF(params.name.c_str()));

    return paramsJobj;
}

JNIEXPORT jboolean JNICALL Java_com_rany010_JavaInterface_setListStruct
  (JNIEnv *env, jobject, jobject paramsInput){
    //���ListObject
    jclass listCls = env->GetObjectClass(paramsInput);

    //�õ�����id
    jmethodID getId = env->GetMethodID(listCls, "get", "(I)Ljava/lang/Object;");
    jmethodID sizeId = env->GetMethodID(listCls, "size", "()I");

    //����sizeId�õ�list��size
    int listSize = env->CallIntMethod(paramsInput, sizeId);

    //����
    for(int i=0;i<listSize;i++)
    {
        //�õ�struct����
        jobject paramsObj = env->CallObjectMethod(paramsInput, getId, i);
        //��ȡparams��
        jclass paramsCls = env->GetObjectClass(paramsObj);

        //���struct��ʵ�ֵ�get������id
        jmethodID idMethodId = env->GetMethodID(paramsCls, "getId", "()I");
        jmethodID nameMethodId = env->GetMethodID(paramsCls, "getName", "()Ljava/lang/String;");

        //����ֶ�ֵ
        jint idValue = env->CallIntMethod(paramsObj, idMethodId);
        jstring nameValue = static_cast<jstring>(env->CallObjectMethod(paramsObj, nameMethodId));

        std::cout<<"[CPP] setStructList index: "<<i<<" id value: "<<idValue<<" name value: "<<env->GetStringUTFChars(nameValue, nullptr)<<std::endl;
    }

    return true;
}

JNIEXPORT jobject JNICALL Java_com_rany010_JavaInterface_getListStruct
(JNIEnv *env, jobject){
    //cpp����Ҫ��֯����
    std::list<Params> listParams;
    Params p1;
    p1.id = 88;
    p1.name = "cpp 88";
    Params p2;
    p2.id = 99;
    p2.name = "cpp 99";
    listParams.push_back(p1);
    listParams.push_back(p2);

    //��ȡһ��java��list��
    jclass listCls = env->FindClass("java/util/ArrayList");
    //��ȡlist�Ĺ��캯��id
    jmethodID initId = env->GetMethodID(listCls, "<init>", "()V");
    //��ȡlist��add����id
    jmethodID addId = env->GetMethodID(listCls, "add","(Ljava/lang/Object;)Z");
    //����һ��list����
    jobject listObj = env->NewObject(listCls, initId, "");

    //��ʼ��װ����
    for(auto iter = listParams.begin();iter!=listParams.end();iter++){
        Params tmpParams = *iter;

        //��ȡParams��
        jclass paramsCls = env->FindClass("com/rany010/Params");
        //��ȡParamsָ���ֶ�id
        jfieldID idId = env->GetFieldID(paramsCls, "id", "I");
        jfieldID nameId = env->GetFieldID(paramsCls, "name", "Ljava/lang/String;");

        //ͨ��params����õ�һ��obj,������ͨ���ֶ�id���ý�ȥ
        jobject paramsObj = env->AllocObject(paramsCls);
        env->SetIntField(paramsObj, idId, tmpParams.id);
        env->SetObjectField(paramsObj, nameId, env->NewStringUTF(tmpParams.name.c_str()));

        //��ÿ����װ�����ݵ�Params obj����list��addid��ӵ�list�󷵻�
        env->CallBooleanMethod(listObj,addId, paramsObj);
    }

    return listObj;
}

JNIEXPORT jboolean JNICALL Java_com_rany010_JavaInterface_setStructList
(JNIEnv *env, jobject, jobject paramsInput){
    Params params;

    jclass paramsCls = env->GetObjectClass(paramsInput);

    //����ֶ�id
    jfieldID idId = env->GetFieldID(paramsCls, "id", "I");
    jfieldID nameId = env->GetFieldID(paramsCls, "name", "Ljava/lang/String;");
    jfieldID bodyListId = env->GetFieldID(paramsCls, "body", "Ljava/util/List;");

    //����id���ֵ
    jint idValue = env->GetIntField(paramsInput, idId);
    jstring nameValue = static_cast<jstring>(env->GetObjectField(paramsInput, nameId));

    params.id = idValue;
    params.name = env->GetStringUTFChars(nameValue,nullptr);

    //ͨ��bodyList��id���obj
    jobject bodyListObj = env->GetObjectField(paramsInput, bodyListId);
    jclass bodyListCls = env->GetObjectClass(bodyListObj);
    //���list���Ӧ��get��size����id
    jmethodID bodyListGetId = env->GetMethodID(bodyListCls, "get", "(I)Ljava/lang/Object;");
    jmethodID bodyListSizeId = env->GetMethodID(bodyListCls, "size", "()I");

    //��ȡ�����list��sizeȻ�����
    jint bodyListSize = env->CallIntMethod(bodyListObj, bodyListSizeId);
    for(int i=0;i<bodyListSize;i++){
        //����ṹ����󣬽����������д������
        BodyIndex bodyIndex;

        jobject bodyIndexObj = env->CallObjectMethod(bodyListObj, bodyListGetId, i);
        jclass bodyIndexCls = env->GetObjectClass(bodyIndexObj);

        //�õ�BodyIndex���ֶ�get����id
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
