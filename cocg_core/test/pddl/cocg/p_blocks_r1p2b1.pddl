(define (problem BW-r1-p2-b1)
    (:domain blocksworld)

    (:objects
        r1 - robot
        ps p1 p2 pg - location
        b1 - block
    )

    (:init
        (robot_at r1 ps)
        (hand_empty r1)
        (unknown
            (block_at b1 p1))
        (unknown
            (block_at b1 p2))
        (oneof
            (block_at b1 p1)
            (block_at b1 p2)
        )
    )
    (:goal
        (and
            (block_at b1 pg)
        )
    )
)

; ff: found plan as follows
; -------------------------------------------------
;   0||0 --- MOVE-TO R1 PS P2 --- SON: 1||0
; -------------------------------------------------
;   1||0 --- SENSE-block_at R1 B1 P2 --- TRUESON: 2||0 --- FALSESON: 2||1
; -------------------------------------------------
;   2||0 --- PICKUP R1 B1 P2 --- SON: 3||0
;   2||1 --- MOVE-TO R1 P2 P1 --- SON: 3||1
; -------------------------------------------------
;   3||0 --- MOVE-WITH-BLOCK R1 B1 P2 PG --- SON: 4||0
;   3||1 --- PICKUP R1 B1 P1 --- SON: 4||1
; -------------------------------------------------
;   4||0 --- PLACE R1 B1 PG --- SON: 5||-1
;   4||1 --- MOVE-WITH-BLOCK R1 B1 P1 PG --- SON: 5||0
; -------------------------------------------------
;   5||0 --- PLACE R1 B1 PG --- SON: 6||-1
; -------------------------------------------------