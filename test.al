int getNum(void)
   begin
      int x;
      write "\nEnter a number :";
      read x;
      return x;
   end

int fibonacci(int input)
begin
   if (input == 0) then
      return 0;`
   endif
   if (input == 1) then
      return 1;
   endif
   return fibonacci(input - 1) + fibonacci(input - 2);
end

void main(void) begin
  int x[4];
  int i, j;

   i = getNum();
   write "Fibonacci of ";
   write i;
   i = fibonacci(i);
   write " = ";
   write i;
   write "\n\n";

end
