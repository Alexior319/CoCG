(define (problem BW-r2-p3-b2)
    (:domain blocksworld)

    (:objects
        r1 r2 - robot
        ps p1 p2 p3 pg - location
        b1 b2 - block
    )

    (:init
        (robot-at r1 ps)
        (robot-at r2 ps)
        (hand-empty r1)
        (hand-empty r2)
        (unknown
            (block-at b1 p1))
        (unknown
            (block-at b1 p2))
        (unknown
            (block-at b2 p1))
        (unknown
            (block-at b2 p3))
        (oneof
            (block-at b1 p1)
            (block-at b1 p2)
        )
        (oneof
            (block-at b2 p1)
            (block-at b2 p3)
        )
    )
    (:goal
        (and
            (block-at b1 pg)
            (block-at b2 pg)
        )
    )
)

; ff: found plan as follows
; -------------------------------------------------
;   0||0 --- MOVE-TO R1 PS P3 --- SON: 1||0
; -------------------------------------------------
;   1||0 --- MOVE-TO R2 PS P2 --- SON: 2||0
; -------------------------------------------------
;   2||0 --- SENSE-BLOCK-AT R2 B1 P2 --- TRUESON: 3||0 --- FALSESON: 3||1
; -------------------------------------------------
;   3||0 --- PICKUP R2 B1 P2 --- SON: 4||0
;   3||1 --- MOVE-TO R2 P2 P1 --- SON: 4||1
; -------------------------------------------------
;   4||0 --- MOVE-WITH-BLOCK R2 B1 P2 PG --- SON: 5||0
;   4||1 --- PICKUP R2 B1 P1 --- SON: 5||1
; -------------------------------------------------
;   5||0 --- PLACE R2 B1 PG --- SON: 6||0
;   5||1 --- MOVE-WITH-BLOCK R2 B1 P1 PG --- SON: 6||1
; -------------------------------------------------
;   6||0 --- SENSE-BLOCK-AT R1 B2 P3 --- TRUESON: 7||0 --- FALSESON: 7||1
;   6||1 --- PLACE R2 B1 PG --- SON: 7||2
; -------------------------------------------------
;   7||0 --- PICKUP R1 B2 P3 --- SON: 8||0
;   7||1 --- MOVE-TO R2 PG P1 --- SON: 8||1
;   7||2 --- SENSE-BLOCK-AT R1 B2 P3 --- TRUESON: 8||2 --- FALSESON: 8||3
; -------------------------------------------------
;   8||0 --- MOVE-WITH-BLOCK R1 B2 P3 PG --- SON: 9||0
;   8||1 --- PICKUP R2 B2 P1 --- SON: 9||1
;   8||2 --- PICKUP R1 B2 P3 --- SON: 9||2
;   8||3 --- MOVE-TO R2 PG P1 --- SON: 9||3
; -------------------------------------------------
;   9||0 --- PLACE R1 B2 PG --- SON: 10||-1
;   9||1 --- MOVE-WITH-BLOCK R2 B2 P1 PG --- SON: 10||0
;   9||2 --- MOVE-WITH-BLOCK R1 B2 P3 PG --- SON: 10||1
;   9||3 --- PICKUP R2 B2 P1 --- SON: 10||2
; -------------------------------------------------
;  10||0 --- PLACE R2 B2 PG --- SON: 11||-1
;  10||1 --- PLACE R1 B2 PG --- SON: 11||-1
;  10||2 --- MOVE-WITH-BLOCK R2 B2 P1 PG --- SON: 11||0
; -------------------------------------------------
;  11||0 --- PLACE R2 B2 PG --- SON: 12||-1
; -------------------------------------------------