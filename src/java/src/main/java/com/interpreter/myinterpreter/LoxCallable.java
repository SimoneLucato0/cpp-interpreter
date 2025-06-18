package com.interpreter.myinterpreter;

import java.util.List;

interface LoxCallable {
    int arity();

    Object call(Interpreter intepreter, List<Object> arguments);
}
