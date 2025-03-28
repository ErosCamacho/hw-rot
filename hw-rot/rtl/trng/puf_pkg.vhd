
package puf_pkg is
    function clog2( n : natural) return integer;
end puf_pkg;

package body puf_pkg is
    function clog2( n : natural) return integer is
        variable tmp : integer := n;
        variable val : integer := 0;
        begin
            while tmp > 1 loop
                val := val + 1;
                tmp := tmp / 2;
            end loop;
        return val;
    end function;
end puf_pkg;