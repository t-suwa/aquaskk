# -*- mode: ruby; coding: utf-8 -*-

class UserDefaults
  def initialize(path)
    @domain = path
    @path = path + '.plist'
  end

  def invoke(command, *args)
    system('defaults', command, @domain, *args)
  end

  def exist?
    FileTest.exist?(@path)
  end

  def read(key)
    invoke('read', key)
  end

  def write(key, value)
    invoke('write', key, value)
  end

  def delete(key)
    invoke('delete', key)
  end

  def rename(oldkey, newkey)
    invoke('rename', oldkey, newkey)
  end

  attr_reader :path
end
