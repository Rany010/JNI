package com.rany010;

import java.util.List;

public class Params {
    public int id;
    public String name;
    public List<BodyIndex> body;

    public Params(){}

    public Params(int id, String name){
        super();
        this.id = id;
        this.name = name;
    }

    public int getId(){return id;}
    public String getName(){return name;}
}
