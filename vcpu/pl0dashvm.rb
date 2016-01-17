# -*- coding: utf-8 -*-
class Pl0dashvm
  MAX_MEM = 1000
  HEAP_START = 800
  
  def initialize(lines)
    init_reg_mem # レジスタとメモリ初期化
    i = 1
    lines.each do |line|
      @memory[i] = line.split(/\s+|\s*,\s*/)
      i += 1
    end
  end
  
  def list_code_area
    i = 1
    @memory[1..(HEAP_START-1)].each do |item|
      break if item == nil
      printf "%4d %s\n", i, [item[0], item[1..2].join(",")].join(" ").strip
      i += 1
    end
  end

  def execute(show = false)
    @reg[:pc] = 1
    ret = -1
    begin
      item = @memory[@reg[:pc]]
      printf "%4d %s\n", @reg[:pc],
        [item[0], item[1..2].join(",")].join(" ").strip if show == true
      ret = execute_line(@memory[@reg[:pc]])
      if (show)
        puts "    " + @reg.map{|reg,val|"%3s:%5d" % [reg,val]}.join(",")
      end
    end while ret > 0
  end

  def execute_line(line)
    (op, opr1, opr2) = line
    case op.upcase
    when "LOAD"
      move(ref_op(opr2), opr1)
      @reg[:pc] += 1
    when "STORE"
      move(ref_op(opr1), opr2)
      @reg[:pc] += 1
    when "PUSH"
      @reg[:sp] -= 1
      move(ref_op(opr1), "#(SP)")
      @reg[:pc] += 1
    when "POP"
      move(@memory[@reg[:sp]], opr1)
      @reg[:sp] += 1
      @reg[:pc] += 1
    when "JMP"
      @reg[:pc] = ref_op(opr1)
    when "JPC"
      if @reg[:c] == 0
        @reg[:pc] = ref_op(opr1)
      else
        @reg[:pc] += 1
      end
    when "PUSHUP"
      @reg[:sp] -=1 
      @reg[:pc] += 1
    when "CALL"
      @reg[:sp] -= 1
      @memory[@reg[:sp]] = @reg[:pc]+1
      @reg[:pc] = ref_op(opr1)
    when "RET"
      @reg[:pc] = @memory[@reg[:sp]]
      @reg[:sp] += 1
      @reg[:sp] += ref_op(opr1)
    when "PRINT"
      print "#{ref_op(opr1)}"
      @reg[:pc] += 1
    when "PRINTLN"
      puts
      @reg[:pc] += 1
    when "END"
      @reg[:pc] = -1
      return -1
    # 以下演算子
    when "PLUS"
      @reg[:c] = @reg[:a]+@reg[:b]
      @reg[:pc] += 1
    when "MINUS"
      @reg[:c] = @reg[:a]-@reg[:b]
      @reg[:pc] += 1
    when "MULTI"
      @reg[:c] = @reg[:a]*@reg[:b]
      @reg[:pc] += 1
    when "DIV"
      @reg[:c] = (@reg[:a] / @reg[:b]).to_i
      @reg[:pc] += 1
    when "CMPODD"
      @reg[:c] = @reg[:a].odd? ? 1 : 0
      @reg[:pc] += 1
    when "CMPEQ"
      @reg[:c] = @reg[:a].to_i == @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    when "CMPNOTEQ"
      @reg[:c] = @reg[:a].to_i != @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    when "CMPLT"
      @reg[:c] = @reg[:a].to_i < @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    when "CMPGT"
      @reg[:c] = @reg[:a].to_i > @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    when "CMPLE"
      @reg[:c] = @reg[:a].to_i <= @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    when "CMPGE"
      @reg[:c] = @reg[:a].to_i >= @reg[:b].to_i ? 1 : 0
      @reg[:pc] += 1
    end
    return 1
  end

  private
  def move(value, sto_op) # valueをsto_opに代入
    case sto_op.downcase
    when /^([a-z]+)$/ # レジスタのはず
      @reg[$1.to_sym] = value
    when /^\#\((\d+)\)$/ # #(数字)のはず
      @memory[$1.to_i] = value
    when /^\#\(([a-z]+)\)$/ # #(レジスタ)のはず
      @memory[@reg[$1.to_sym]] = value
    when /^\#\(fp([\+|\-]\d+)\)$/ # #(FP(+|-)数字)のはず
      @memory[@reg[:fp]+$1.to_i] = value
    else
      puts "ERROR"
      exit 1;
    end
  end

  def ref_op(opr)
    case opr.downcase
    when /^(\d+)$/    # 数字
      # puts $1
      return $1.to_i
    when /^([a-z]+)$/ # レジスタのはず
      # puts $1
      return @reg[$1.to_sym]
    when /^\#\((\d+)\)$/ # #(数字)のはず
      # puts $1
      return @memory[$1.to_i]
    when /^\#\(([a-z]+)\)$/ # #(レジスタ)のはず
      # puts $1.to_sym
      return @memory[@reg[$1.to_sym]]
    when /^\#\(fp([\+|\-]\d+)\)$/ # #(FP(+|-)数字)のはず
      # puts "#{@reg[:fp]+$1.to_i}"
      return @memory[@reg[:fp]+$1.to_i]
    else
      puts "ERROR"
      exit 1;
    end
  end

  def init_reg_mem
    @memory = Array.new(MAX_MEM+1)
    @memory[0] = "FirstLine"
    @reg = Hash.new
    @reg[:a] = 0
    @reg[:b] = 0
    @reg[:c] = 0
    @reg[:sp] = MAX_MEM+1
    @reg[:fp] = MAX_MEM+1
    @reg[:pc] = 0
  end
end

if ARGV[0] == nil
  puts "#{$0} asm_filename\n"
  exit 1;
end

file = File.open(ARGV[0])
lines = file.readlines
vm = Pl0dashvm.new(lines)
# vm.list_code_area
vm.execute
