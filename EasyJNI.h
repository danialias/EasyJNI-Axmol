/****************************************************************************
 Copyright (c) 2015 Victor Komarov
 https://github.com/fnz
  
 Adapted to Axmol Engine by danialias - 2024/03/11 (yyyy/mm/dd)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "jni.h"
#include "jni/JniHelper.h"
#include "platform/PlatformConfig.h"

#include <vector>
#include <unordered_map>

USING_NS_AX;

class EasyJNI {
public:
    template <typename... Ts>
    static void callStaticVoidMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")V";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            t.env->CallStaticVoidMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
    }

    template <typename... Ts>
    static bool callStaticBooleanMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        jboolean jret = JNI_FALSE;
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")Z";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            jret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return (jret == JNI_TRUE);
    }

    template <typename... Ts>
    static int callStaticIntMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        jint ret = 0;
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")I";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            ret = t.env->CallStaticIntMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return ret;
    }

    template <typename... Ts>
    static jlong callStaticLongMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        jlong ret = 0;
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")J";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            ret = t.env->CallStaticLongMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return ret;
    }

    template <typename... Ts>
    static float callStaticFloatMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        jfloat ret = 0.0;
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")F";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            ret = t.env->CallStaticFloatMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return ret;
    }

    template <typename... Ts>
    static double callStaticDoubleMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        jdouble ret = 0.0;
        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")D";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            ret = t.env->CallStaticDoubleMethod(t.classID, t.methodID, convert(t, xs)...);
            t.env->DeleteLocalRef(t.classID);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return ret;
    }

    template <typename... Ts>
    static std::string callStaticStringMethod(const std::string& className, const std::string& methodName, Ts... xs) {
        std::string ret;

        JniMethodInfo t;
        std::string signature = "(" + std::string(getJNISignature(xs...)) + ")Ljava/lang/String;";
        if (JniHelper::getStaticMethodInfo(t, className.c_str(), methodName.c_str(), signature.c_str())) {
            jstring jret = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, convert(t, xs)...);
            ret = JniHelper::jstring2string(jret);
            t.env->DeleteLocalRef(t.classID);
            t.env->DeleteLocalRef(jret);
            deleteLocalRefs(t.env);
        } else {
            reportError(className, methodName, signature);
        }
        return ret;
    }

private:
    static jstring convert(JniMethodInfo& t, const char* x);

    static jstring convert(JniMethodInfo& t, const std::string& x);

    template <typename T>
    static T convert(JniMethodInfo&, T x) {
        return x;
    }

    static std::unordered_map<JNIEnv*, std::vector<jobject>> localRefs;

    static void deleteLocalRefs(JNIEnv* env);

    static std::string getJNISignature() {
        return "";
    }

    static std::string getJNISignature(bool) {
        return "Z";
    }

    static std::string getJNISignature(char) {
        return "C";
    }

    static std::string getJNISignature(short) {
        return "S";
    }

    static std::string getJNISignature(int) {
        return "I";
    }

    static std::string getJNISignature(long) {
        return "J";
    }

    static std::string getJNISignature(float) {
        return "F";
    }

    static std::string getJNISignature(double) {
        return "D";
    }

    static std::string getJNISignature(const char*) {
        return "Ljava/lang/String;";
    }

    static std::string getJNISignature(const std::string&) {
        return "Ljava/lang/String;";
    }

    template <typename T>
    static std::string getJNISignature(T x) {
        // This template should never be instantiated
        static_assert(sizeof(x) == 0, "Unsupported argument type");
        return "";
    }

    template <typename T, typename... Ts>
    static std::string getJNISignature(T x, Ts... xs) {
        return getJNISignature(x) + getJNISignature(xs...);
    }

    static void reportError(const std::string& className, const std::string& methodName, const std::string& signature);
};

#endif
