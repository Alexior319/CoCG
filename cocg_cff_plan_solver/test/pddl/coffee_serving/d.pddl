(define (domain coffee_serving)
    (:requirements :strips :typing :disjunctive-preconditions)

    (:types
        coffee robot location
    )
    (:predicates
        (robot_at ?r - robot ?p - location)
        (hand_empty ?r - robot)
        (hold_coffee ?r - robot ?c - coffee)
        (coffee_at ?c - coffee ?p - location)
        (stirred ?c - coffee)
    )

    ; (:action locate_robot_at
    ;     :parameters (?r - robot ?p - location)
    ;     :observe
    ;     (robot_at ?r ?p)
    ; )

    ; (:action sense_hand_empty
    ;     :parameters (?r - robot)
    ;     :observe
    ;     (hand_empty ?r)
    ; )

    ; (:action sense_hold_coffee
    ;     :parameters (?r - robot ?c - coffee)
    ;     :observe
    ;     (hold_coffee ?r ?c)
    ; )

    (:action sense_coffee_at
        :parameters (?r - robot ?c - coffee ?p - location)
        :precondition (and
            (robot_at ?r ?p)
        )
        :observe
        (coffee_at ?c ?p)
    )

    (:action goto
        :parameters (?r - robot ?pf - location ?pt - location)
        :precondition (and (robot_at ?r ?pf))
        :effect (and (not (robot_at ?r ?pf)) (robot_at ?r ?pt))
    )

    ; (:action move_with_coffee
    ;     :parameters (?r - robot ?c - coffee ?pf - location ?pt - location)
    ;     :precondition (and (robot_at ?r ?pf) (hold_coffee ?r ?c))
    ;     :effect (and (not (robot_at ?r ?pf)) (robot_at ?r ?pt))
    ; )

    (:action pickup
        :parameters (?r - robot ?c - coffee ?p - location)
        :precondition (and (robot_at ?r ?p) (coffee_at ?c ?p) (hand_empty ?r))
        :effect (and (not (coffee_at ?c ?p)) (not (hand_empty ?r)) (hold_coffee ?r ?c))
    )

    (:action stir
        :parameters (?r - robot ?c - coffee ?p - location)
        :precondition (and (hold_coffee ?r ?c))
        :effect (and (stirred ?c))
    )

    (:action place
        :parameters (?r - robot ?c - coffee ?p - location)
        :precondition (and (robot_at ?r ?p) (hold_coffee ?r ?c) (stirred ?c))
        :effect (and (not (hold_coffee ?r ?c)) (coffee_at ?c ?p))
    )

)

;; Notice: predicate or action names should not contain '-' (subtract)
;; or, parser will collide with the numeric