(define (problem CM_R1_C6)
    (:domain coffee_serving)

    (:objects
        r1 r2 - robot
        ps p1 p2 pg - location
        c1 c2 c3 c4 c5 c6 c7 c8 c9 c10 - coffee
    )

    (:init
        (robot_at r1 ps)
        (hand_empty r1)
        (robot_at r2 ps)
        (hand_empty r2)
        (unknown
            (coffee_at c1 p1))
        (unknown
            (coffee_at c1 p2))

        (oneof
            (coffee_at c1 p1)
            (coffee_at c1 p2)
        )
        (unknown
            (coffee_at c2 p1))
        (unknown
            (coffee_at c2 p2))

        (oneof
            (coffee_at c2 p1)
            (coffee_at c2 p2)
        )
        (unknown
            (coffee_at c3 p1))
        (unknown
            (coffee_at c3 p2))

        (oneof
            (coffee_at c3 p1)
            (coffee_at c3 p2)
        )
        (unknown
            (coffee_at c4 p1))
        (unknown
            (coffee_at c4 p2))

        (oneof
            (coffee_at c4 p1)
            (coffee_at c4 p2)
        )
        (unknown
            (coffee_at c5 p1))
        (unknown
            (coffee_at c5 p2))

        (oneof
            (coffee_at c5 p1)
            (coffee_at c5 p2)
        )
        (unknown
            (coffee_at c6 p1))
        (unknown
            (coffee_at c6 p2))
        (oneof
            (coffee_at c6 p1)
            (coffee_at c6 p2)
        )
        (unknown
            (coffee_at c7 p1))
        (unknown
            (coffee_at c7 p2))
        (oneof
            (coffee_at c7 p1)
            (coffee_at c7 p2)
        )
        (unknown
            (coffee_at c8 p1))
        (unknown
            (coffee_at c8 p2))
        (oneof
            (coffee_at c8 p1)
            (coffee_at c8 p2)
        )
        (unknown
            (coffee_at c9 p1))
        (unknown
            (coffee_at c9 p2))
        (oneof
            (coffee_at c9 p1)
            (coffee_at c9 p2)
        )
        (unknown
            (coffee_at c10 p1))
        (unknown
            (coffee_at c10 p2))
        (oneof
            (coffee_at c10 p1)
            (coffee_at c10 p2)
        )
    )
    (:goal
        (and
            (coffee_at c1 pg)
            (coffee_at c2 pg)
            (coffee_at c3 pg)
            (coffee_at c4 pg)
            (coffee_at c5 pg)
            (coffee_at c6 pg)
            (coffee_at c7 pg)
            (coffee_at c8 pg)
            (coffee_at c9 pg)
            (coffee_at c10 pg)
        )
    )
)