; good2.as

             .entry      TARGET

mcro   jumpmacro
      jmp     $31
      call            TARGET
mcroend

        add      $0   ,  $1    ,    $2

TARGET:     subi    $10  ,   -500  ,  $10
  
    
      jumpmacro

            .dw    100000   ,   -99999
hlt