def chkBrackets(s : List[Char]) : Boolean ={
  def chk(s : List[Char], rk : Int) : Boolean ={
    if (s.isEmpty){
      rk == 0
    } else if (s.head == '('){
             chk(s.tail, rk + 1)    
           } else if (s.head == ')') {
                    chk(s.tail, rk - 1)
                  } else chk(s.tail, rk)
  }
  chk(s, 0)
}
