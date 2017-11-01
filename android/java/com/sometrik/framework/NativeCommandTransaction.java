package com.sometrik.framework;

import java.util.ArrayList;

class NativeCommandTransaction {
  private ArrayList<NativeCommand> commands = new ArrayList<NativeCommand>();

  public NativeCommandTransaction() { }
    
  public void addCommand(NativeCommand command) {
    commands.add(command);
  }
    
  public ArrayList<NativeCommand> getCommands() {
    return commands;
  }
}
